#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);


    qSettings = new QuestionSettings();


    db = new QSqlDatabase();
    model = new QStringListModel(this);
    languages = new QStringListModel(this);
    model->setStringList(slist);
    languages->setStringList(llist);


    ui->combo_learning_language->setModel(languages);
    ui->combo_native_language->setModel(languages);
    QStringList list = (QStringList()<< "1" << "2" << "3" << "4" << "5" << "6" << "7" << "8" << "9" << "10");
    ui->comboBox_practise_number_of_tries->addItems(list);




    //Initialize database
    init_db();

    //Initialize question settings
    init_qSettings();


    // Set the window stack to point to the main_menu
    ui->window_stacker->setCurrentIndex(0);


    // Set background picture

    QPixmap bkgnd("C:/Users/christian/desktop/white_background_langualizer.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);


    //Turn of borders
    //setWindowFlags( Qt::FramelessWindowHint);

}

MainWindow::~MainWindow()
{
    delete ui;
    delete db;
    delete qry;
    delete qSettings;
    delete addBooks;
    delete addChapter;
    delete addWords;
}
// ___________________________________DATABASE METHODS___________________________________

void MainWindow::init_db()
{
    *db = QSqlDatabase::addDatabase("QMYSQL");
    db->setHostName("localhost");
    db->setDatabaseName("langualizer");
    db->setUserName("root");
    db->setPassword("xz99wydpmkj9ry");

    qry = new QSqlQuery();

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
            db->close();
        }
        else
        {
            QMessageBox::information(this,"ERROR", "Languages quary failed.");
        }

    }
}

void MainWindow::init_qSettings()
{

    //Setting up QStringListModels for chapter and book selection
    slmAvailableBooks = new QStringListModel(this);
    slmAvailableChapters = new QStringListModel(this);
    slmSelectedBooks = new QStringListModel(this);
    slmSelectedChapters = new QStringListModel(this);

    slmAvailableBooks->setStringList(slAvailableBooks);
    slmAvailableChapters->setStringList(slAvailableChapters);
    slmSelectedBooks->setStringList(slSelectedBooks);
    slmSelectedChapters->setStringList(slSelectedChapters);

    slmAvailableBooks->setStringList(slAvailableBooks);
    slmAvailableChapters->setStringList(slAvailableChapters);
    slmSelectedBooks->setStringList(slSelectedBooks);
    slmSelectedChapters->setStringList(slSelectedChapters);

    ui->listview_practise_configure_available_books->setModel(slmAvailableBooks);
    ui->treeview_practise_configure_available_chapters->setModel(slmAvailableChapters);
    ui->listview_practise_configure_selected_books->setModel(slmSelectedBooks);
    ui->treeview_practise_configure_selected_chapters->setModel(slmSelectedChapters);

    //make views uneditable
    ui->listview_practise_configure_available_books->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->listview_practise_configure_selected_books->setEditTriggers(QAbstractItemView::NoEditTriggers);

    ui->treeview_practise_configure_available_chapters->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->treeview_practise_configure_selected_chapters->setEditTriggers(QAbstractItemView::NoEditTriggers);

    if(db->open())
    {
        if(qry->exec("SELECT * FROM settings WHERE settings_id = '1';"))
        {
            if(qry->next())
            {
                qSettings->setNoun(qry->value(1).toBool());
                ui->checkBox_practise_noun->setChecked(qry->value(1).toBool());
                qSettings->setAdj(qry->value(2).toBool());
                ui->checkBox_practise_adjective->setChecked(qry->value(2).toBool());
                qSettings->setAdv(qry->value(3).toBool());
                ui->checkBox_practise_adverb->setChecked(qry->value(3).toBool());
                qSettings->setPron(qry->value(4).toBool());
                ui->checkBox_practise_pronoun->setChecked(qry->value(4).toBool());
                qSettings->setPrep(qry->value(5).toBool());
                ui->checkBox_practise_preposition->setChecked(qry->value(5).toBool());
                qSettings->setVerb(qry->value(6).toBool());
                ui->checkBox_practise_verb->setChecked(qry->value(6).toBool());
                qSettings->setOther(qry->value(7).toBool());
                ui->checkBox_practise_other->setChecked(qry->value(7).toBool());
                qSettings->setNToC(qry->value(8).toBool());
                ui->checkBox_practise_native_to_chosen->setChecked(qry->value(8).toBool());
                qSettings->setCToN(qry->value(9).toBool());
                ui->checkBox_practise_chosen_to_native->setChecked(qry->value(9).toBool());
                qSettings->setTries(qry->value(10).toInt());
                ui->comboBox_practise_number_of_tries->setCurrentText(qry->value(10).toString());
                qSettings->setTips(qry->value(11).toBool());
                ui->checkBox_practise_show_tips->setChecked(qry->value(11).toBool());
                qSettings->setAlternative(qry->value(12).toBool());
                ui->checkBox_practise_alternative_text->setChecked(qry->value(12).toBool());
                qSettings->setNative(qry->value(13).toString());
                ui->combo_native_language->setCurrentText(qry->value(13).toString());
                qSettings->setChosen(qry->value(14).toString());
                ui->combo_learning_language->setCurrentText(qry->value(14).toString());
                ui->label_selected_language->setText(qry->value(14).toString());

            }
        }
        else
        {
            QMessageBox::information(this,"ERROR", "At qSettings_init: Query failed.");
        }
    }
    else
    {
        QMessageBox::information(this,"ERROR", "At qSettings_init: Database didn't open.");
    }

    native_language = ui->combo_native_language->currentText();
    selected_language = ui->combo_learning_language->currentText();
    qSettings->setNative(native_language);
    qSettings->setChosen(selected_language);
    ui->label_selected_language->setText(selected_language);
    update_available_books();





}

void MainWindow::update_available_books()
{
    if(db->open())
    {
        QSqlQuery *qry = new QSqlQuery();
        //Setup available books
        QString sqry = "SELECT DISTINCT book_name FROM books WHERE book_language = :language;";
        qry->prepare(sqry);
        qry->bindValue(":language",selected_language);


        if(qry->exec())
        {
            while(qry->next())
            {
                slmAvailableBooks->insertRow(slmAvailableBooks->rowCount());
                QModelIndex index = slmAvailableBooks->index(slmAvailableBooks->rowCount()-1);
                slmAvailableBooks->setData(index, qry->value(0));
            }
        }
        else
        {
            QMessageBox::information(this,"Error","Books quary failed.");
        }
        delete qry;
        db->close();

    }
    else
    {
        QMessageBox::information(this,"Error","Database didn't open.");
    }

}

void MainWindow::update_available_chapters()
{

}



void MainWindow::changeEvent(QEvent *e)
{
    QMainWindow::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}

// ___________________________________BUTTON METHODS______________________________________
// _______________________________________________________________________________________



//--------------------------------------MAIN MENU--------------------------------------

void MainWindow::on_button_exit_clicked()
{
    //close program
    this->close();
}

void MainWindow::on_pushButton_clicked()
{
    this->close();
}


void MainWindow::on_button_options_clicked()
{
    //show options menu
    ui->window_stacker->setCurrentIndex(3);
}

void MainWindow::on_button_information_clicked()
{
    //show information menu

    ui->window_stacker->setCurrentIndex(2);
}

void MainWindow::on_button_start_clicked()
{
    //show start menu
    ui->window_stacker->setCurrentIndex(1);

}

//--------------------------------------MAIN MENU--------------------------------------

void MainWindow::on_button_back_clicked()
{
    //back to main_menu
    ui->window_stacker->setCurrentIndex(0);
}


//----------------------------------INFORMATION MENU-----------------------------------

void MainWindow::on_button_back_2_clicked()
{
    //back to main_menu

    ui->window_stacker->setCurrentIndex(0);
}


//-----------------------------------OPTIONS MENU--------------------------------------


void MainWindow::on_button_back_3_clicked()
{
    //back to main_menu
    ui->window_stacker->setCurrentIndex(0);
}


//---------------------------------START LEARNING MENU---------------------------------

void MainWindow::on_button_practise_clicked()
{
    ui->window_stacker->setCurrentIndex(6);
}

void MainWindow::on_button_test_clicked()
{
    ui->window_stacker->setCurrentIndex(7);

}

void MainWindow::on_button_vocabulary_clicked()
{
    ui->window_stacker->setCurrentIndex(5);
}

void MainWindow::on_button_select_language_clicked()
{
    ui->window_stacker->setCurrentIndex(4);
}


//-----------------------------------VOCABULARY MENU-----------------------------------

void MainWindow::on_button_back_vocabulary_clicked()
{
    ui->window_stacker->setCurrentIndex(1);
}

void MainWindow::on_button_words_clicked()
{
    ui->window_stacker->setCurrentIndex(8);
}

void MainWindow::on_button_chapters_clicked()
{
    ui->window_stacker->setCurrentIndex(9);
}

void MainWindow::on_button_books_clicked()
{
    ui->window_stacker->setCurrentIndex(10);
}


//------------------------------------PRACTISE MENU------------------------------------

void MainWindow::on_button_back_practise_clicked()
{
    ui->window_stacker->setCurrentIndex(1);
}


void MainWindow::on_button_relax_mode_clicked()
{
    //ui->window_stacker->setCurrentIndex(14);
    relaxMode = new RelaxMode(0,db,qSettings);
    relaxMode->show();
    //this->hide();
}

void MainWindow::on_button_time_attack_clicked()
{
    ui->window_stacker->setCurrentIndex(15);
}

void MainWindow::on_button_hard_core_clicked()
{
    ui->window_stacker->setCurrentIndex(16);
}

void MainWindow::on_button_configure_practise_settings_clicked()
{
    ui->window_stacker->setCurrentIndex(17);
}

//--------------------------------------TEST_MENU--------------------------------------

void MainWindow::on_button_back_test_clicked()
{
    ui->window_stacker->setCurrentIndex(1);
}


//---------------------------------SELECT_LANGUAGE_MENU--------------------------------


void MainWindow::on_button_back_select_language_clicked()
{
    ui->window_stacker->setCurrentIndex(1);
}

void MainWindow::on_button_select_language_save_changes_clicked()
{
    native_language = ui->combo_native_language->currentText();
    selected_language = ui->combo_learning_language->currentText();
    qSettings->setNative(native_language);
    qSettings->setChosen(selected_language);
    ui->label_selected_language->setText(selected_language);
    ui->info_label->setText("Settings saved!");
    QTimer::singleShot(2000, this, SLOT(updateInfoLabel()));
    update_available_books();
    save_settings_to_database();




}

//---------------------------------WORDS_MENU--------------------------------------

void MainWindow::on_button_back_words_clicked()
{
    ui->window_stacker->setCurrentIndex(5);
}

void MainWindow::on_button_add_words_clicked()
{
    addWords = new AddWords(0,db);
    addWords->show();
}

void MainWindow::on_button_modify_words_clicked()
{
    ui->window_stacker->setCurrentIndex(12);
}

void MainWindow::on_button_all_words_clicked()
{
    ui->window_stacker->setCurrentIndex(13);
}


//---------------------------------CHAPTERS_MENU------------------------------------


void MainWindow::on_button_back_chapters_clicked()
{
    ui->window_stacker->setCurrentIndex(5);
}

void MainWindow::on_button_add_chapter_clicked()
{
    addChapter = new AddChapter(0,db);
    addChapter->show();
}

void MainWindow::on_button_modify_chapter_clicked()
{

}

void MainWindow::on_button_browse_chapters_clicked()
{

}

//---------------------------------BOOKS_MENU---------------------------------------


void MainWindow::on_button_back_books_clicked()
{
    ui->window_stacker->setCurrentIndex(5);
}

void MainWindow::on_button_add_books_clicked()
{
    addBooks = new AddBooks(0,db);
    addBooks->show();
}

void MainWindow::on_button_modify_books_clicked()
{

}

void MainWindow::on_button_browse_books_clicked()
{

}

//---------------------------------ADD_WORDS_MENU-------------------------------------

void MainWindow::on_button_back_add_words_clicked()
{
    ui->window_stacker->setCurrentIndex(8);
}


//---------------------------------MODIFY_WORDS_MENU----------------------------------

void MainWindow::on_button_back_modify_words_clicked()
{
    ui->window_stacker->setCurrentIndex(8);
}

//---------------------------------ALL_WORDS_MENU-------------------------------------


void MainWindow::on_button_back_all_words_clicked()
{
    ui->window_stacker->setCurrentIndex(8);
}

//---------------------------------RELAX MODE MENU-------------------------------------

void MainWindow::on_button_back_relax_mode_clicked()
{
    ui->window_stacker->setCurrentIndex(6);
}



//---------------------------------TIME ATTACK MENU------------------------------------

void MainWindow::on_button_back_time_attack_clicked()
{
    ui->window_stacker->setCurrentIndex(6);
}



//---------------------------------HARD CORE MENU--------------------------------------

void MainWindow::on_button_back_hard_core_clicked()
{
    ui->window_stacker->setCurrentIndex(6);
}



//--------------------------CONFIGURE PRACTISE SETTINGS MENU---------------------------


void MainWindow::on_button_back_configure_practise_settings_clicked()
{
    ui->window_stacker->setCurrentIndex(6);
}

void MainWindow::on_pushButton_save_practise_settings_clicked()
{
    qSettings->setNoun(ui->checkBox_practise_noun->isChecked());
    qSettings->setVerb(ui->checkBox_practise_verb->isChecked());
    qSettings->setAdj(ui->checkBox_practise_adjective->isChecked());
    qSettings->setAdv(ui->checkBox_practise_adverb->isChecked());
    qSettings->setPron(ui->checkBox_practise_pronoun->isChecked());
    qSettings->setPrep(ui->checkBox_practise_preposition->isChecked());
    qSettings->setOther(ui->checkBox_practise_other->isChecked());

    qSettings->setNToC(ui->checkBox_practise_native_to_chosen->isChecked());
    qSettings->setCToN(ui->checkBox_practise_chosen_to_native->isChecked());
    qSettings->setTips(ui->checkBox_practise_show_tips->isChecked());
    qSettings->setTries(ui->comboBox_practise_number_of_tries->currentText().toInt());
    qSettings->setAlternative(ui->checkBox_practise_alternative_text->isChecked());

    //get chapter ids for selected chapters
    QString book = ui->listview_practise_configure_selected_books->currentIndex().data().toString();
    qSettings->setBook(book);

    QStringList selectedChapters_ids;

    if (slSelectedChaptersList.length() > 0 )
    {
        if(db->open())
        {
            QSqlQuery *qry = new QSqlQuery();
            QString sqry = "SELECT DISTINCT chapter_id FROM chapters,books WHERE chapters.book_id = books.book_id AND book_name = :book AND chapter_name = :chapter;";
            qry->prepare(sqry);
            qry->bindValue(":book",book);
            for(int i = 0; i < slSelectedChaptersList.length();i++)
            {
                qry->bindValue(":chapter",slSelectedChaptersList[i]);
                if(qry->exec())
                {
                    if(qry->next())
                    {
                        selectedChapters_ids << qry->value(0).toString();
                    }
                }
                else
                {
                    QMessageBox::information(this,"ERROR","Query failed.");
                }
            }
            db->close();
            delete qry;
        }
        else
        {
            QMessageBox::information(this,"ERROR","Database not open");
        }

    }

    qSettings->setChapters(selectedChapters_ids);





    save_settings_to_database();
    ui->info_label->setText("Settings saved!");
    QTimer::singleShot(2000, this, SLOT(updateInfoLabel()));
}

void MainWindow::updateInfoLabel()
{
    ui->info_label->setText("");
}


void MainWindow::on_button_practise_configure_book_add_clicked()
{
    //Add the selected book to the selected books view
    slmSelectedBooks->insertRow(slmSelectedBooks->rowCount());
    QModelIndex index = slmSelectedBooks->index(slmSelectedBooks->rowCount()-1);
    slmSelectedBooks->setData(index, ui->listview_practise_configure_available_books->currentIndex().data());

    //Then delete the the transfered book from available books
    slmAvailableBooks->removeRow(ui->listview_practise_configure_available_books->currentIndex().row());


}

void MainWindow::on_button_practise_configure_book_remove_clicked()
{
    //Add the selected book to the available books view
    slmAvailableBooks->insertRow(slmAvailableBooks->rowCount());
    QModelIndex index = slmAvailableBooks->index(slmAvailableBooks->rowCount()-1);
    slmAvailableBooks->setData(index, ui->listview_practise_configure_selected_books->currentIndex().data());

    //Then delete the the transfered book from selected books
    slmSelectedBooks->removeRow(ui->listview_practise_configure_selected_books->currentIndex().row());
}

void MainWindow::on_button_practise_configure_chapter_add_clicked()
{
    //Add the selected chapter to the selected chapters view
    slmSelectedChapters->insertRow(slmSelectedChapters->rowCount());
    QModelIndex index = slmSelectedChapters->index(slmSelectedChapters->rowCount()-1);
    slmSelectedChapters->setData(index,ui->treeview_practise_configure_available_chapters->currentIndex().data() );

    //Add selection to the chapterlist which is sent to questionsettings
    slSelectedChaptersList << ui->treeview_practise_configure_available_chapters->currentIndex().data().toString();

    //Then delete the the transfered chapter from available chapters
    slmAvailableChapters->removeRow(ui->treeview_practise_configure_available_chapters->currentIndex().row());
}

void MainWindow::on_button_practise_configure_chapter_remove_clicked()
{
    //Add the selected chapter to the available chapters view
    slmAvailableChapters->insertRow(slmAvailableChapters->rowCount());
    QModelIndex index = slmAvailableChapters->index(slmAvailableChapters->rowCount()-1);
    slmAvailableChapters->setData(index, ui->treeview_practise_configure_selected_chapters->currentIndex().data());


    //Delete the removed chapter from the list passed to questionsettings
    QString toBeDeleted = ui->treeview_practise_configure_selected_chapters->currentIndex().data().toString();
    QStringList temp;
    for(int i = 0;i<slSelectedChaptersList.length();i++)
    {
        if(slSelectedChaptersList[i] != toBeDeleted)
        {
            temp << slSelectedChaptersList[i];
        }

    }
    slSelectedChaptersList = temp;


    //Then delete the the transfered chapter from selected chapters
    slmSelectedChapters->removeRow(ui->treeview_practise_configure_selected_chapters->currentIndex().row());
}

void MainWindow::on_listview_practise_configure_selected_books_activated(const QModelIndex &index)
{
    slmAvailableChapters->removeRows(0,slmAvailableChapters->rowCount());
    slmSelectedChapters->removeRows(0,slmSelectedChapters->rowCount());

    if(db->open())
    {
        QSqlQuery *qry = new QSqlQuery();
        //Setup available chapters
        QString sqry = "SELECT DISTINCT chapter_name,chapter_id FROM chapters,books WHERE chapters.book_id = books.book_id AND books.book_id = (SELECT book_id FROM books WHERE book_name = :book);";
        qry->prepare(sqry);
        qry->bindValue(":language",selected_language);

        //get the name of the selected book
        QAbstractItemModel *model = ui->listview_practise_configure_selected_books->model();
        QString selection = model->index(0,0).data( Qt::DisplayRole ).toString() ;
        qry->bindValue(":book",selection);


        if(qry->exec())
        {
            while(qry->next())
            {
                slmAvailableChapters->insertRow(slmAvailableChapters->rowCount());
                QModelIndex index = slmAvailableChapters->index(slmAvailableChapters->rowCount()-1);
                slmAvailableChapters->setData(index, qry->value(0));


            }
        }
        else
        {
            QMessageBox::information(this,"Error","Books quary failed.");
        }
        delete qry;
        db->close();

    }
    else
    {
        QMessageBox::information(this,"Error","Database didn't open.");
    }
}

// ___________________________________CUSTOM WINDOW METHODS___________________________________
// ___________________________________________________________________________________________

void MainWindow::mousePressEvent(QMouseEvent *e)
{
  if (!isMaximized() &&
      e->button() == Qt::LeftButton &&
      ui->title->geometry().contains(e->pos())) {
    window_drag_start_pos = e->pos();
  }
}

void MainWindow::mouseReleaseEvent(QMouseEvent *e)
{
  window_drag_start_pos = QPoint(0, 0);
}

void MainWindow::mouseMoveEvent(QMouseEvent *e)
{
  if (!window_drag_start_pos.isNull()) {
    move(pos() + e->pos() - window_drag_start_pos);
  }
}

void MainWindow::on_minimize_clicked()
{
  showMinimized();
}

void MainWindow::on_maximize_clicked()
{
  if (isMaximized()) {
    showNormal();
  } else {
    showMaximized();
  }
}
//_____________________________________________________________________________________________


void MainWindow::save_settings_to_database()
{
    if(db->open())
    {
        //Save all the settings to database except books and chapters

        QSqlQuery *qry = new QSqlQuery();

        QString sqry = "UPDATE `langualizer`.`settings` SET `noun`= :noun, `adjective`= :adj, `adverb`= :adv, `pronoun`= :pron, `prepostion`= :prep, `verb`= :verb, `other`= :other, `ntoc`= :ntoc, `cton`= :cton, `ntries`= :tries, `showtips`= :tips, `alternative` = :alternative,`native`= :native, `chosen`= :chosen WHERE `settings_id`='1';";
        qry->prepare(sqry);
        qry->bindValue(":noun",qSettings->noun());
        qry->bindValue(":adj",qSettings->adj());
        qry->bindValue(":adv",qSettings->adv());
        qry->bindValue(":pron",qSettings->pron());
        qry->bindValue(":prep",qSettings->prep());
        qry->bindValue(":verb",qSettings->verb());
        qry->bindValue(":other",qSettings->other());
        qry->bindValue(":ntoc",qSettings->nToC());
        qry->bindValue(":cton",qSettings->cToN());
        qry->bindValue(":tries",qSettings->tries());
        qry->bindValue(":tips",qSettings->tips());
        qry->bindValue(":alternative",qSettings->alternative());
        qry->bindValue(":native",qSettings->native());
        qry->bindValue(":chosen",qSettings->chosen());

        if(!qry->exec())
        {
            QMessageBox::information(this,"Error","At save settings: Query failed.");
        }
        delete qry;
    }
    else
    {
        QMessageBox::information(this,"ERROR","At save settings: Database didn't open.");
    }
}













