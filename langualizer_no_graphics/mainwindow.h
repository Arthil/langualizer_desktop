#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QSqlQuery>
#include <QSql>
#include <QSqlError>
#include <QMessageBox>
#include <QStringListModel>
#include <QMouseEvent>
#include <QStringListModel>
#include <QTreeView>
#include <QWindow>

#include "addchapter.h"
#include "addbooks.h"
#include "addwords.h"
#include "relaxmode.h"
#include "questionsettings.h"


namespace Ui
{
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT


protected:
    void changeEvent(QEvent *e);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();



private slots:
    void on_button_exit_clicked();

    void on_button_options_clicked();

    void on_button_information_clicked();

    void on_button_start_clicked();

    void on_button_back_clicked();

    void on_button_back_2_clicked();

    void on_button_back_3_clicked();

    void on_button_select_language_clicked();

    void on_button_back_select_language_clicked();

    void on_pushButton_clicked();

    void on_button_back_vocabulary_clicked();

    void on_button_vocabulary_clicked();

    void on_button_back_practise_clicked();

    void on_button_practise_clicked();

    void on_button_back_test_clicked();

    void on_button_test_clicked();

    void on_button_words_clicked();

    void on_button_chapters_clicked();

    void on_button_books_clicked();

    void on_button_back_words_clicked();

    void on_button_back_chapters_clicked();

    void on_button_back_books_clicked();

    void on_button_add_words_clicked();

    void on_button_modify_words_clicked();

    void on_button_all_words_clicked();

    void on_button_back_add_words_clicked();

    void on_button_back_modify_words_clicked();

    void on_button_back_all_words_clicked();

    void on_button_relax_mode_clicked();

    void on_button_time_attack_clicked();

    void on_button_hard_core_clicked();

    void on_button_configure_practise_settings_clicked();

    void on_button_back_relax_mode_clicked();

    void on_button_back_time_attack_clicked();

    void on_button_back_hard_core_clicked();

    void on_button_back_configure_practise_settings_clicked();

    void on_button_add_chapter_clicked();

    void on_button_add_books_clicked();

    void on_button_modify_books_clicked();

    void on_button_browse_books_clicked();

    void on_button_modify_chapter_clicked();

    void on_button_browse_chapters_clicked();

    void on_pushButton_save_practise_settings_clicked();

    void updateInfoLabel();
    void on_button_select_language_save_changes_clicked();

    void on_button_practise_configure_book_add_clicked();

    void on_button_practise_configure_book_remove_clicked();

    void on_listview_practise_configure_selected_books_activated(const QModelIndex &index);

    void on_button_practise_configure_chapter_add_clicked();

    void on_button_practise_configure_chapter_remove_clicked();

private:
    Ui::MainWindow *ui;
    void init_db();
    void init_qSettings();
    void update_available_books();
    void update_available_chapters();
    void save_settings_to_database();

    QSqlDatabase *db;
    QPoint window_drag_start_pos;
    QString native_language;
    QString selected_language;

    QSqlQuery *qry;
    QStringListModel *languages;
    QStringListModel *model;
    QStringList slist;
    QStringList llist;

    //for book and chapter selection
    QStringListModel *slmAvailableBooks;
    QStringListModel *slmAvailableChapters;
    QStringListModel *slmSelectedBooks;
    QStringListModel *slmSelectedChapters;

    QStringList slAvailableBooks;
    QStringList slAvailableChapters;
    QStringList slSelectedBooks;
    QStringList slSelectedChapters;

    QStringList slSelectedChaptersList;



    AddChapter *addChapter;
    AddBooks *addBooks;
    AddWords *addWords;
    RelaxMode *relaxMode;
    QuestionSettings *qSettings;

    void mousePressEvent(QMouseEvent *e);
    void mouseReleaseEvent(QMouseEvent *e);
    void mouseMoveEvent(QMouseEvent *e);
    void on_minimize_clicked();
    void on_maximize_clicked();


};

#endif // MAINWINDOW_H
