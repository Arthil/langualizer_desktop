#ifndef ADDCHAPTER_H
#define ADDCHAPTER_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringListModel>
#include <QMessageBox>
#include <QSqlError>
#include <QTimer>

namespace Ui {
class AddChapter;
}

class AddChapter : public QWidget
{
    Q_OBJECT

public:
    explicit AddChapter(QWidget *parent = 0, QSqlDatabase *DB = 0);
    ~AddChapter();

private slots:

    void on_button_clear_clicked();

    void on_button_cancel_clicked();

    void on_button_add_chapter_clicked();

    void updateInfoLabel();
private:
    void init_available_books();

    Ui::AddChapter *ui;
    QSqlDatabase *db;

    QStringListModel *slmodel;
    QStringList slist;
    QSqlQuery *qry;

};

#endif // ADDCHAPTER_H
