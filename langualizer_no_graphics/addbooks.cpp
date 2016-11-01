#include "addbooks.h"
#include "ui_addbooks.h"

AddBooks::AddBooks(QWidget *parent,QSqlDatabase *DB) :
    QWidget(parent),
    ui(new Ui::AddBooks)
{
    ui->setupUi(this);
    qry = new QSqlQuery();
    languages = new QStringListModel(this);
    languages->setStringList(llist);
    ui->comboBox_language->setModel(languages);
    db = DB;

    init_languages();
}

AddBooks::~AddBooks()
{
    delete ui;
    delete qry;
}


void AddBooks::on_button_cancel_clicked()
{
    this->close();
}

void AddBooks::on_button_clear_clicked()
{
    ui->book_name->setText("");
    ui->book_description->setText("");
}

void AddBooks::on_button_add_book_clicked()
{
    //Add a book into the database


    if(db->open())
    {
        QString sqry = "INSERT INTO `langualizer`.`books` (`book_name`, `book_description` ,`book_language` ) VALUES (:book, :description, :language);";
        qry->prepare(sqry);
        qry->bindValue(":book",ui->book_name->text());
        qry->bindValue(":description", ui->book_description->toPlainText());
        qry->bindValue(":language",ui->comboBox_language->currentText());

        if(qry->exec())
        {

        }
        else
        {
            QMessageBox::information(this,"ERROR", "Quary failed.");

        }




    on_button_clear_clicked();
    ui->info_label->setText("Book added!");
    QTimer::singleShot(5000, this, SLOT(updateInfoLabel()));
    }
    else
    {
        QMessageBox::information(this,"ERROR", "Database didn't open.");
        //db->close();
    }
}


void AddBooks::updateInfoLabel()
{
    ui->info_label->setText("");
}

void AddBooks::init_languages()
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
            QMessageBox::information(this,"ERROR", "Languages quary failed at addbooks.");

            db->close();
        }
        db->close();
    }
    else
    {
        QMessageBox::information(this,"ERROR", "Database didn't open at addbooks.");
    }
}
