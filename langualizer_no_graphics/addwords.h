#ifndef ADDWORDS_H
#define ADDWORDS_H

#include <QWidget>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include <QMessageBox>
#include <QStringListModel>
#include <QTimer>

namespace Ui {
class AddWords;
}

class AddWords : public QWidget
{
    Q_OBJECT

public:
    explicit AddWords(QWidget *parent = 0, QSqlDatabase *DB = 0);
    ~AddWords();
private slots:
    void updateInfoLabel();

    void on_button_cancel_clicked();

    void on_button_clear_clicked();

    void on_button_add_word_clicked();

    void on_language_choice2_currentTextChanged();

    void on_language_choice1_currentTextChanged();


    void on_book_choice_currentIndexChanged(const QString &arg1);

    void on_button_add_row_language1_clicked();

    void on_button_add_row_language1_2_clicked();

    void on_button_add_row_language1_3_clicked();

    void on_button_add_row_language1_4_clicked();

    void on_button_delete_row_language1_2_clicked();

    void on_button_delete_row_language1_3_clicked();

    void on_button_delete_row_language1_4_clicked();

    void on_button_delete_row_language1_5_clicked();

    void on_button_add_row_language2_clicked();

    void on_button_add_row_language2_2_clicked();

    void on_button_add_row_language2_3_clicked();

    void on_button_add_row_language2_4_clicked();

    void on_button_delete_row_language2_2_clicked();

    void on_button_delete_row_language2_3_clicked();

    void on_button_delete_row_language2_4_clicked();

    void on_button_delete_row_language2_5_clicked();

private:
    Ui::AddWords *ui;

    QSqlDatabase *db;
    QSqlQuery *qry;
    QSqlQuery *qry2;
    QSqlQuery *qry3;

    QStringListModel *languages;
    QStringList llist;
    QStringListModel *slmBooks;
    QStringList slBooks;
    QStringListModel *slmChapters;
    QStringList slChapters;


    void init_languages();
    void hide_language_input_lines();
};

#endif // ADDWORDS_H
