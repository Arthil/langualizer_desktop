#include "addchapter.h"
#include "ui_addchapter.h"

AddChapter::AddChapter(QWidget *parent,QSqlDatabase *DB) :
    QWidget(parent),
    ui(new Ui::AddChapter)
{
    ui->setupUi(this);
    qry = new QSqlQuery();
    db = DB;
    slmodel = new QStringListModel(this);
    slmodel->setStringList(slist);
    ui->book_choice->setModel(slmodel);

    init_available_books();
}

AddChapter::~AddChapter()
{
    delete ui;
    delete qry;
}

void AddChapter::init_available_books()
{

    //Get all the available books and set them to the combobox

    if(db->open())
    {
        if(qry->exec("SELECT DISTINCT book_name FROM books;"))
        {
            while(qry->next())
            {
                slmodel->insertRow(slmodel->rowCount());
                QModelIndex index = slmodel->index(slmodel->rowCount()-1);
                slmodel->setData(index, qry->value(0));
            }
            db->close();
        }
        else
        {
            QMessageBox::information(this,"ERROR", "Book quary failed.");
        }

    }
}


void AddChapter::on_button_clear_clicked()
{
    ui->chapter_name->setText("");
    ui->chapter_description->setText("");
}

void AddChapter::on_button_cancel_clicked()
{
    this->close();
}

void AddChapter::on_button_add_chapter_clicked()
{
    //Add the chapter to the database



    if(db->open())
    {
        QString sqry = "INSERT INTO `langualizer`.`chapters` (`chapter_name`, `chapter_description`, `book_id`) VALUES (:name, :description, (SELECT DISTINCT book_id FROM books WHERE book_name = :book));";
        qry->prepare(sqry);
        qry->bindValue(":book", ui->book_choice->currentText());
        qry->bindValue(":name", ui->chapter_name->text());
        qry->bindValue(":description", ui->chapter_description->toPlainText());

        if(qry->exec())
        {
        db->close();
        }
        else
        {
            QMessageBox::information(this,"ERROR", "Quary failed.");
            db->close();
        }




    on_button_clear_clicked();
    ui->info_label->setText("Chapter added!");
    QTimer::singleShot(5000, this, SLOT(updateInfoLabel()));

    }
    else
    {
        QMessageBox::information(this,"ERROR", "Database didn't open.");
    }
}


void AddChapter::updateInfoLabel()
{
    ui->info_label->setText("");
}
