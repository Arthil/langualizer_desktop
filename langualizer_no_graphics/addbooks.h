#ifndef ADDBOOKS_H
#define ADDBOOKS_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringListModel>
#include <QMessageBox>
#include <QSqlError>
#include <QTimer>

namespace Ui {
class AddBooks;
}

class AddBooks : public QWidget
{
    Q_OBJECT

public:
    explicit AddBooks(QWidget *parent = 0, QSqlDatabase *DB = 0);
    ~AddBooks();

private slots:
    void on_button_cancel_clicked();

    void on_button_clear_clicked();

    void on_button_add_book_clicked();

    void updateInfoLabel();

private:

    void init_languages();

    Ui::AddBooks *ui;
    QSqlDatabase *db;
    QSqlQuery *qry;

    QStringListModel *languages;
    QStringList llist;

};

#endif // ADDBOOKS_H
