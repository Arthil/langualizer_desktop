#ifndef RELAXMODE_H
#define RELAXMODE_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QStringListModel>
#include <QMessageBox>
#include <QSqlError>
#include <QTimer>

#include <ctime>
#include <cstdlib>

#include "questionsettings.h"


namespace Ui {
class RelaxMode;
}

class RelaxMode : public QWidget
{
    Q_OBJECT

public:
    explicit RelaxMode(QWidget *parent = 0, QSqlDatabase *DB = 0, QuestionSettings *qSett = 0);
    ~RelaxMode();

private slots:
    void on_lineEdit_answer_returnPressed();

    void on_pushButton_check_clicked();

    void on_pushButton_skip_clicked();

    void on_pushButton_show_answer_clicked();

    void on_pushButton_start_clicked();

    void response_update();

    void call_next();

    void on_checkBox_show_alternative_form_toggled(bool checked);

    void on_pushButton_end_clicked();

private:
    void init_questions();
    QStringList get_answer();
    void next_question();
    QString get_alternative_text(QString &word);

    inline int random(int a,int b){return rand() % b + a;}


    Ui::RelaxMode *ui;
    QSqlDatabase *db;
    QSqlQuery *qry;
    QSqlQuery *qry2;

    //QStringListModel *slmQuestions;
    QStringList slQuestionsNative;
    QStringList slQuestionsChosen;
    QString currentTargetLanguage;
    QString currentQuestion;

    int triesLeft;
    int correctAnswers;
    int wrongAnswers;

    QuestionSettings *qSettings;

    QStringList chapters;

    QString correctAnswer;
    bool inputEnabled;
};

#endif // RELAXMODE_H
