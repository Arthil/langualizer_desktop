#include "addwords.h"
#include "ui_addwords.h"

AddWords::AddWords(QWidget *parent,QSqlDatabase *DB) :
    QWidget(parent),
    ui(new Ui::AddWords)
{
    ui->setupUi(this);

    qry = new QSqlQuery();
    qry2 = new QSqlQuery();
    qry3 = new QSqlQuery();

    languages = new QStringListModel(this);
    languages->setStringList(llist);
    ui->language_choice1->setModel(languages);
    ui->language_choice2->setModel(languages);

    slmBooks = new QStringListModel(this);
    slmBooks->setStringList(slBooks);
    ui->book_choice->setModel(slmBooks);

    slmChapters = new QStringListModel(this);
    slmChapters->setStringList(slChapters);
    ui->chapter_choice->setModel(slmChapters);


    QStringList list = (QStringList()<< "Noun" << "Verb" << "Adjective" << "Adverb" << "Pronoun" << "Preposition" << "Other");
    ui->type_choice->addItems(list);

    db = DB;
    init_languages();
    hide_language_input_lines();

}

AddWords::~AddWords()
{
    delete ui;
    delete qry;
    delete qry2;
    delete qry3;
}

void AddWords::on_button_cancel_clicked()
{
    //Close the window

    this->close();
}

void AddWords::init_languages()
{


    //Get all the language options for the database and add them to the comboboxes for selection

    if(db->open())
    {
        if(qry->exec("SELECT DISTINCT language_name FROM languages;"))
        {
            while(qry->next())
            {
                languages->insertRow(languages->rowCount());
                QModelIndex index = languages->index(languages->rowCount()-1);
                languages->setData(index, qry->value(0));
            }

        }
        else
        {
            QMessageBox::information(this,"ERROR", "Languages quary failed.");

            db->close();
        }
        ui->label_language2->setText(ui->language_choice2->currentText());
        ui->label_language1->setText(ui->language_choice1->currentText());


        //Initialize the comboboxes for available books


        if(qry->exec("SELECT DISTINCT book_name FROM books;"))
        {
            while(qry->next())
            {
                slmBooks->insertRow(slmBooks->rowCount());
                QModelIndex index = slmBooks->index(slmBooks->rowCount()-1);
                slmBooks->setData(index, qry->value(0));
            }

        }
        else
        {
            QMessageBox::information(this,"ERROR", "Books quary failed.");
            db->close();
        }

        //Initialize the chapters available for the first book

        QString qstr = "SELECT DISTINCT chapter_name FROM chapters WHERE book_id = (SELECT DISTINCT book_id FROM books WHERE book_name = :book);";
        qry->prepare(qstr);
        qry->bindValue(":book",ui->book_choice->currentText());

        if(qry->exec())
        {
            while(qry->next())
            {
                slmChapters->insertRow(slmChapters->rowCount());
                QModelIndex index = slmChapters->index(slmChapters->rowCount()-1);
                slmChapters->setData(index, qry->value(0));
            }

        }
        else
        {
            QMessageBox::information(this,"ERROR", "Chapters quary failed.");
            db->close();
        }



    }



}

void AddWords::on_language_choice2_currentTextChanged()
{
    //update language 1 label

    ui->label_language2->setText(ui->language_choice2->currentText());
}

void AddWords::on_language_choice1_currentTextChanged()
{
    //update language 2 label

    ui->label_language1->setText(ui->language_choice1->currentText());
}

void AddWords::on_button_clear_clicked()
{
    // Clear all the entry fields

    ui->line_language1->setText("");
    ui->line_language2->setText("");
    ui->text_description->setText("");
}

void AddWords::on_button_add_word_clicked()
{
    //Add the first word to the database



    if(db->open())
    {
        //Check if word already exists

        QString sqry = "SELECT COUNT(word_id) FROM words WHERE word_text = :word AND (SELECT language_id from languages where language_name = :language)= language_id ";
        qry3->prepare(sqry);
        qry3->bindValue(":word",ui->line_language1->text());
        qry3->bindValue(":language",ui->language_choice1->currentText());
        if(qry3->exec())
        {
            qry3->next();

            // If word doesn't exist, add it to the database.
            if(qry3->value(0).toInt() == 0)
            {
                sqry = "INSERT INTO `langualizer`.`words` (`language_id`, `word_text`, `description`, `book`, `word_type` ,`word_alternative`) VALUES ((SELECT language_id from languages where language_name = :language), :word, :description, :book, :type, :alternative);";
                qry3->prepare(sqry);
                qry3->bindValue(":language",ui->language_choice1->currentText());
                qry3->bindValue(":word",ui->line_language1->text());
                qry3->bindValue(":description",ui->text_description->toPlainText());
                qry3->bindValue(":book",ui->book_choice->currentText());
                qry3->bindValue(":chapter",ui->chapter_choice->currentText());
                qry3->bindValue(":type", ui->type_choice->currentText());

                if(!(ui->lineEdit_alternative_text_1->text() == ""))
                {
                    qry3->bindValue(":alternative", ui->lineEdit_alternative_text_1->text());
                }
                else
                {
                    qry3->bindValue(":alternative", "");
                }

                if(!qry3->exec())
                {
                    QMessageBox::information(this,"ERROR", "Quary failed at word 1 insert to words.");
                    QMessageBox::information(this,"ERROR", qry3->lastError().text() );
                    db->close();
                    return;
                }

            }
            else
            {
                QMessageBox::information(this,"ERROR", "Word 1 already exists in the database");
            }
        }
        else
        {
            QMessageBox::information(this,"ERROR", "Quary failed at word 1 check if word 1 exists.");
            QMessageBox::information(this,"ERROR", qry3->lastError().text() );
            db->close();
            return;
        }
//-----------------------------------------------------------------------------------------------------------------------------------------------------------

        // Check if word is already in selected chapter "wordin"
        sqry = "SELECT COUNT(word_id) FROM words WHERE word_text = :word AND (SELECT language_id from languages where language_name = :language)= language_id AND word_id IN (SELECT DISTINCT word_id FROM wordin WHERE chapter_id = (SELECT DISTINCT chapter_id FROM chapters WHERE chapter_name = :chapter and book_id = (SELECT DISTINCT book_id FROM books WHERE book_name = :book)))";
        qry3->prepare(sqry);
        qry3->bindValue(":word",ui->line_language1->text());
        qry3->bindValue(":language",ui->language_choice1->currentText());
        qry3->bindValue(":book",ui->book_choice->currentText());
        qry3->bindValue(":chapter",ui->chapter_choice->currentText());
        if(qry3->exec())
        {
            qry3->next();
            if(qry3->value(0).toInt() == 0)
            {
                sqry = "INSERT INTO `langualizer`.`wordin` (`word_id`, `chapter_id`) VALUES ((SELECT DISTINCT word_id FROM words WHERE (SELECT language_id from languages where language_name = :language) = language_id AND word_text = :word) ,(SELECT DISTINCT chapter_id FROM chapters WHERE chapter_name = :chapter and book_id = (SELECT DISTINCT book_id FROM books WHERE book_name = :book)));";
                qry3->prepare(sqry);
                qry3->bindValue(":language",ui->language_choice1->currentText());
                qry3->bindValue(":word",ui->line_language1->text());
                qry3->bindValue(":book",ui->book_choice->currentText());
                qry3->bindValue(":chapter",ui->chapter_choice->currentText());
                if(!qry3->exec())
                {
                    QMessageBox::information(this,"ERROR", "Quary failed at word 1 insert to wordin.");
                    QMessageBox::information(this,"ERROR", qry3->lastError().text() );
                }
            }
            else
            {
                QMessageBox::information(this,"ERROR", "Word 1 already in selected chapter");
            }
        }
        else
        {
            QMessageBox::information(this,"ERROR", "Quary failed at word 1 check if exists in wordin");
            QMessageBox::information(this,"ERROR", qry3->lastError().text() );
        }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

    //Add the second word to the database

        sqry = "SELECT COUNT(word_id) FROM words WHERE word_text = :word AND (SELECT language_id from languages where language_name = :language)= language_id ";
        qry3->prepare(sqry);
        qry3->bindValue(":word",ui->line_language2->text());
        qry3->bindValue(":language",ui->language_choice2->currentText());
        if(qry3->exec())
        {
            qry3->next();

            // If word doesn't exist, add it to the database.
            if(qry3->value(0).toInt() == 0)
            {
                sqry = "INSERT INTO `langualizer`.`words` (`language_id`, `word_text`, `description`, `book`, `word_type` ,`word_alternative`) VALUES ((SELECT language_id from languages where language_name = :language), :word, :description, :book, :type, :alternative);";
                qry3->prepare(sqry);
                qry3->bindValue(":language",ui->language_choice2->currentText());
                qry3->bindValue(":word",ui->line_language2->text());
                qry3->bindValue(":description",ui->text_description->toPlainText());
                qry3->bindValue(":book",ui->book_choice->currentText());
                qry3->bindValue(":chapter",ui->chapter_choice->currentText());
                qry3->bindValue(":type", ui->type_choice->currentText());

                if(!(ui->lineEdit_alternative_text_2->text() == ""))
                {
                    qry3->bindValue(":alternative", ui->lineEdit_alternative_text_2->text());
                }
                else
                {
                    qry3->bindValue(":alternative", "");
                }

                if(!qry3->exec())
                {
                    QMessageBox::information(this,"ERROR", "Quary failed at word 2 insert to words.");
                    QMessageBox::information(this,"ERROR", qry3->lastError().text() );
                    db->close();
                    return;
                }

            }
            else
            {
                QMessageBox::information(this,"ERROR", "Word 2 already exists in the database");
            }
        }
        else
        {
            QMessageBox::information(this,"ERROR", "Quary failed at word 2 check if word 2 exists.");
            QMessageBox::information(this,"ERROR", qry3->lastError().text() );
            db->close();
            return;
        }

//-----------------------------------------------------------------------------------------------------------------------------------------------------------

        // Check if word is already in selected chapter "wordin"
        sqry = "SELECT COUNT(word_id) FROM words WHERE word_text = :word AND (SELECT language_id from languages where language_name = :language)= language_id AND word_id IN (SELECT DISTINCT word_id FROM wordin WHERE chapter_id = (SELECT DISTINCT chapter_id FROM chapters WHERE chapter_name = :chapter and book_id = (SELECT DISTINCT book_id FROM books WHERE book_name = :book)))";
        qry3->prepare(sqry);
        qry3->bindValue(":word",ui->line_language2->text());
        qry3->bindValue(":language",ui->language_choice2->currentText());
        qry3->bindValue(":book",ui->book_choice->currentText());
        qry3->bindValue(":chapter",ui->chapter_choice->currentText());
        if(qry3->exec())
        {
            qry3->next();
            if(qry3->value(0).toInt() == 0)
            {
                sqry = "INSERT INTO `langualizer`.`wordin` (`word_id`, `chapter_id`) VALUES ((SELECT DISTINCT word_id FROM words WHERE (SELECT language_id from languages where language_name = :language) = language_id AND word_text = :word) ,(SELECT DISTINCT chapter_id FROM chapters WHERE chapter_name = :chapter and book_id = (SELECT DISTINCT book_id FROM books WHERE book_name = :book)));";
                qry3->prepare(sqry);
                qry3->bindValue(":language",ui->language_choice2->currentText());
                qry3->bindValue(":word",ui->line_language2->text());
                qry3->bindValue(":book",ui->book_choice->currentText());
                qry3->bindValue(":chapter",ui->chapter_choice->currentText());
                if(!qry3->exec())
                {
                    QMessageBox::information(this,"ERROR", "Quary failed at word 2 insert to wordin.");
                    QMessageBox::information(this,"ERROR", qry3->lastError().text() );
                }
            }
            else
            {
                QMessageBox::information(this,"ERROR", "Word 2 already in selected chapter");
            }
        }
        else
        {
            QMessageBox::information(this,"ERROR", "Quary failed at word 2 check if exists in wordin");
            QMessageBox::information(this,"ERROR", qry3->lastError().text() );
        }





//-----------------------------------------------------------------------------------------------------------------------------------------------------------

    //Add the translation for the words


    sqry = "INSERT INTO `langualizer`.`translations` (`word_id1`, `word_id2`) VALUES ((SELECT word_id FROM words WHERE word_text = :word1), (SELECT word_id FROM words WHERE word_text = :word2));";
    qry3->prepare(sqry);
    qry3->bindValue(":word1",ui->line_language1->text());
    qry3->bindValue(":word2",ui->line_language2->text());


        if(qry3->exec())
        {
            db->close();
        }
        else
        {
            QMessageBox::information(this,"ERROR", "Translation already exists.");
            db->close();

        }

    on_button_clear_clicked();
    ui->info_label->setText("Word added!");
    QTimer::singleShot(5000, this, SLOT(updateInfoLabel()));

    }
    else
    {
        QMessageBox::information(this,"ERROR", "Database didn't open.");
    }

}

void AddWords::updateInfoLabel()
{
    ui->info_label->setText("");
}



void AddWords::on_book_choice_currentIndexChanged(const QString &arg1)
{
    //Initialize the chapters for the selected book

    slmChapters->removeRows(0,slmChapters->rowCount());
    if(db->open())
    {
        QString qstr = "SELECT DISTINCT chapter_name FROM chapters WHERE book_id = (SELECT DISTINCT book_id FROM books WHERE book_name = :book);";
        qry2->prepare(qstr);
        qry2->bindValue(":book",ui->book_choice->currentText());

        if(qry2->exec())
        {
            while(qry2->next())
            {
                slmChapters->insertRow(slmChapters->rowCount());
                QModelIndex index = slmChapters->index(slmChapters->rowCount()-1);
                slmChapters->setData(index, qry2->value(0));
            }

        }
        else
        {
            QMessageBox::information(this,"ERROR", "chapter quary failed.");
            db->close();
        }
    }
    else
    {
        QMessageBox::information(this,"ERROR", "Database didn't open.");
    }
}



void AddWords::hide_language_input_lines()
{
    //hide left word input thingies
    ui->button_delete_row_language1_2->hide();
    ui->line_language1_2->hide();
    ui->button_add_row_language1_2->hide();
    ui->button_delete_row_language1_3->hide();
    ui->line_language1_3->hide();
    ui->button_add_row_language1_3->hide();
    ui->button_delete_row_language1_4->hide();
    ui->line_language1_4->hide();
    ui->button_add_row_language1_4->hide();
    ui->button_delete_row_language1_5->hide();
    ui->line_language1_5->hide();

    //hide right word input thingies
    ui->button_delete_row_language2_2->hide();
    ui->line_language2_2->hide();
    ui->button_add_row_language2_2->hide();
    ui->button_delete_row_language2_3->hide();
    ui->line_language2_3->hide();
    ui->button_add_row_language2_3->hide();
    ui->button_delete_row_language2_4->hide();
    ui->line_language2_4->hide();
    ui->button_add_row_language2_4->hide();
    ui->button_delete_row_language2_5->hide();
    ui->line_language2_5->hide();

}

//Left side word add/delete buttons

void AddWords::on_button_add_row_language1_clicked()
{
    ui->button_add_row_language1->hide();
    ui->button_delete_row_language1_2->show();
    ui->line_language1_2->show();
    ui->button_add_row_language1_2->show();
}

void AddWords::on_button_add_row_language1_2_clicked()
{
    ui->button_add_row_language1_2->hide();
    ui->button_delete_row_language1_3->show();
    ui->line_language1_3->show();
    ui->button_add_row_language1_3->show();
    ui->button_delete_row_language1_2->hide();
}

void AddWords::on_button_add_row_language1_3_clicked()
{
    ui->button_add_row_language1_3->hide();
    ui->button_delete_row_language1_4->show();
    ui->line_language1_4->show();
    ui->button_add_row_language1_4->show();
    ui->button_delete_row_language1_3->hide();
}

void AddWords::on_button_add_row_language1_4_clicked()
{
    ui->button_add_row_language1_4->hide();
    ui->button_delete_row_language1_5->show();
    ui->line_language1_5->show();
    ui->button_delete_row_language1_4->hide();
}

void AddWords::on_button_delete_row_language1_2_clicked()
{
    ui->button_add_row_language1->show();
    ui->button_delete_row_language1_2->hide();
    ui->line_language1_2->hide();
    ui->button_add_row_language1_2->hide();
}

void AddWords::on_button_delete_row_language1_3_clicked()
{
    ui->button_add_row_language1_2->show();
    ui->button_delete_row_language1_3->hide();
    ui->line_language1_3->hide();
    ui->button_add_row_language1_3->hide();
    ui->button_delete_row_language1_2->show();
}

void AddWords::on_button_delete_row_language1_4_clicked()
{
    ui->button_add_row_language1_3->show();
    ui->button_delete_row_language1_4->hide();
    ui->line_language1_4->hide();
    ui->button_add_row_language1_4->hide();
    ui->button_delete_row_language1_3->show();
}

void AddWords::on_button_delete_row_language1_5_clicked()
{
    ui->button_add_row_language1_4->show();
    ui->button_delete_row_language1_5->hide();
    ui->line_language1_5->hide();
    ui->button_delete_row_language1_4->show();
}


//Right side word add/delete buttons


void AddWords::on_button_add_row_language2_clicked()
{
    ui->button_add_row_language2->hide();
    ui->button_delete_row_language2_2->show();
    ui->line_language2_2->show();
    ui->button_add_row_language2_2->show();
}

void AddWords::on_button_add_row_language2_2_clicked()
{
    ui->button_add_row_language2_2->hide();
    ui->button_delete_row_language2_3->show();
    ui->line_language2_3->show();
    ui->button_add_row_language2_3->show();
    ui->button_delete_row_language2_2->hide();
}

void AddWords::on_button_add_row_language2_3_clicked()
{
    ui->button_add_row_language2_3->hide();
    ui->button_delete_row_language2_4->show();
    ui->line_language2_4->show();
    ui->button_add_row_language2_4->show();
    ui->button_delete_row_language2_3->hide();
}

void AddWords::on_button_add_row_language2_4_clicked()
{
    ui->button_add_row_language2_4->hide();
    ui->button_delete_row_language2_5->show();
    ui->line_language2_5->show();
    ui->button_delete_row_language2_4->hide();
}

void AddWords::on_button_delete_row_language2_2_clicked()
{
    ui->button_add_row_language2->show();
    ui->button_delete_row_language2_2->hide();
    ui->line_language2_2->hide();
    ui->button_add_row_language2_2->hide();
}

void AddWords::on_button_delete_row_language2_3_clicked()
{
    ui->button_add_row_language2_2->show();
    ui->button_delete_row_language2_3->hide();
    ui->line_language2_3->hide();
    ui->button_add_row_language2_3->hide();
    ui->button_delete_row_language2_2->show();
}

void AddWords::on_button_delete_row_language2_4_clicked()
{
    ui->button_add_row_language2_3->show();
    ui->button_delete_row_language2_4->hide();
    ui->line_language2_4->hide();
    ui->button_add_row_language2_4->hide();
    ui->button_delete_row_language2_3->show();
}

void AddWords::on_button_delete_row_language2_5_clicked()
{
    ui->button_add_row_language2_4->show();
    ui->button_delete_row_language2_5->hide();
    ui->line_language2_5->hide();
    ui->button_delete_row_language2_4->show();
}
