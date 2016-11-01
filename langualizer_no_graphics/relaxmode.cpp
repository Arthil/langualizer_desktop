#include "relaxmode.h"
#include "ui_relaxmode.h"

RelaxMode::RelaxMode(QWidget *parent,QSqlDatabase *DB, QuestionSettings *qSett) :
    QWidget(parent),
    ui(new Ui::RelaxMode)
{
    ui->setupUi(this);


    db = DB;

    qSettings = qSett;

    qry = new QSqlQuery();
    qry2 = new QSqlQuery();

    //Setting the seed for random numbers
    std::srand(time(NULL));

    //set background image

    QPixmap bkgnd("C:/Users/christian/desktop/white_background_langualizer.jpg");
    bkgnd = bkgnd.scaled(this->size(), Qt::IgnoreAspectRatio);
    QPalette palette;
    palette.setBrush(QPalette::Background, bkgnd);
    this->setPalette(palette);

    inputEnabled = true;
    ui->checkBox_show_alternative_form->setChecked(qSettings->alternative());
    ui->stackedWidget->setCurrentIndex(0);
    ui->label_response->setText("");
    ui->label_answer->setText("");
    ui->label_alternative_text->setText("");
    ui->label_hint_word->setText("");
    ui->label_correct_count->setText("0");
    ui->label_wrong_count->setText("0");

    chapters = qSettings->chapters();

    correctAnswers = 0;
    wrongAnswers = 0;



    init_questions();


}

RelaxMode::~RelaxMode()
{
    delete ui;
    delete qry;
    delete qry2;
}

void RelaxMode::init_questions()
{
    if(db->open())
    {
        QString chapterQueryText = "";
        QString noun;
        QString verb;
        QString adj;
        QString adv;
        QString other;
        QString pron;
        QString prep;

        for(int i = 0; i < chapters.length();i++)
        {
            chapterQueryText += " or chapters.chapter_id = '" + QString(chapters[i]) + "' ";
        }

        if(qSettings->noun() == true)
        {
            noun = " or word_type = 'Noun' ";
        }
        else
        {
            noun = "";
        }

        if(qSettings->verb() == true)
        {
            verb = " or word_type = 'Verb' ";
        }
        else
        {
            verb = "";
        }
        if(qSettings->adj() == true)
        {
            adj = " or word_type = 'Adjective' ";
        }
        else
        {
            adj = "";
        }
        if(qSettings->adv() == true)
        {
            adv = " or word_type = 'Adverb' ";
        }
        else
        {
            adv = "";
        }
        if(qSettings->other() == true)
        {
            other = " or word_type = 'Other' ";
        }
        else
        {
            other = "";
        }
        if(qSettings->prep() == true)
        {
            prep = " or word_type = 'Preposition' ";
        }
        else
        {
            prep = "";
        }
        if(qSettings->pron() == true)
        {
            pron = " or word_type = 'Pronoun' ";
        }
        else
        {
            pron = "";
        }

        if(qSettings->nToC() == true)
        {
            QString sqry = "SELECT DISTINCT word_text FROM words,languages WHERE words.language_id = languages.language_id and languages.language_id = (SELECT language_id FROM languages WHERE language_name = '" + qSettings->native()+"') and (word_type = 'empty'" + verb + noun + adj + adv + pron + prep + other + ") AND word_text IN (SELECT DISTINCT word_text FROM words,chapters,wordin WHERE words.word_id = wordin.word_id AND wordin.chapter_id = chapters.chapter_id AND (chapters.chapter_id = '' " + chapterQueryText +"));";

            if(qry->exec(sqry))
            {

                while(qry->next())
                {
                    slQuestionsNative << qry->value(0).toString();

                }

            }
            else
            {
                QMessageBox::information(this,"ERROR", "Quary failed at nToC get questions.");
                db->close();
            }
        }
        if(qSettings->cToN() == true)
        {
            QString sqry = "SELECT DISTINCT word_text FROM words,languages WHERE words.language_id = languages.language_id and languages.language_id = (SELECT language_id FROM languages WHERE language_name = '" + qSettings->chosen()+"') and (word_type = 'empty'" + verb + noun + adj + adv + pron + prep + other + ") AND word_text IN (SELECT DISTINCT word_text FROM words,chapters,wordin WHERE words.word_id = wordin.word_id AND wordin.chapter_id = chapters.chapter_id AND (chapters.chapter_id = '' " + chapterQueryText +"));";


            if(qry2->exec(sqry))
            {

                while(qry2->next())
                {
                    slQuestionsChosen << qry2->value(0).toString();

                }
                db->close();
            }
            else
            {
                QMessageBox::information(this,"ERROR", "Quary failed at cToN get questions.");
                db->close();
            }
        }

    }
    else
    {
        QMessageBox::information(this,"ERROR", "Database didn't open.");
    }
}

QStringList RelaxMode::get_answer()
{
    //Gets a list of right answers
    QSqlQuery *qry = new QSqlQuery();

    if(db->open())
    {
        QString sQuery = "SELECT DISTINCT word_text FROM languages,words,translations WHERE words.language_id = languages.language_id and languages.language_id = (SELECT DISTINCT language_id from languages where language_name = :language) and (word_id1 = (SELECT word_id from words where word_text = :word )  or (SELECT word_id from words where word_text = :word) = word_id2) and (word_id1 = word_id  or word_id = word_id2);";
        qry->prepare(sQuery);

        qry->bindValue(":word", currentQuestion);
        qry->bindValue(":language", currentTargetLanguage);

        if(qry->exec())
        {
            QStringList right_answers;
            while(qry->next())
            {
                right_answers << qry->value(0).toString();
            }
            return right_answers;
        }
        else
        {
            QMessageBox::information(this,"Error", "Quary failed at get answer.");
            QStringList empty;
            delete qry;
            return empty;
        }
    }
    else
    {
        QStringList empty;
        delete qry;
        return empty;
    }
    delete qry;
}


void RelaxMode::on_lineEdit_answer_returnPressed()
{
    QString userAnswer = ui->lineEdit_answer->text();
    ui->lineEdit_answer->setText("");
    bool correct;
    correct = false;
    if (inputEnabled == true && userAnswer != "")
    {
        triesLeft -= 1;
        QStringList rightAnswers = get_answer();
        for(int i = 0; i < rightAnswers.length();i++)
        {
            if(rightAnswers.at(i) == userAnswer)
            {

                correct = true;
            }
        }
        if (correct == true)
        {
            correctAnswers += 1;
            ui->label_response->setText("Correct!");
            QTimer::singleShot(750, this, SLOT(response_update()));
            next_question();

        }
        else
        {

            ui->label_response->setText("Wrong!");
            QTimer::singleShot(2000, this, SLOT(response_update()));
            if(triesLeft < 1)
            {
                wrongAnswers += 1;
                on_pushButton_show_answer_clicked();
            }
        }
    }
}

void RelaxMode::response_update()
{
    ui->label_response->setText("");
    ui->label_answer->setText("");
    ui->label_hint_word->setText("");
    ui->label_correct_count->setText(QString::number(correctAnswers));
    ui->label_wrong_count->setText(QString::number(wrongAnswers));

}

void RelaxMode::on_pushButton_check_clicked()
{
    on_lineEdit_answer_returnPressed();
}

void RelaxMode::on_pushButton_skip_clicked()
{
    next_question();

}

void RelaxMode::on_pushButton_show_answer_clicked()
{
    inputEnabled = false;
    QStringList answerList = get_answer();
    QString answer = "";
    if(answerList.length() > 0)
    {
        foreach(QString str,answerList)
        answer += str + "\n";
    }
    else
    {
        answer = "No answers found";
    }

    ui->label_answer->setText("Right answers: " + answer);
    QTimer::singleShot(2000, this, SLOT(call_next()));
    QTimer::singleShot(2000, this, SLOT(response_update()));
}

void RelaxMode::call_next()
{
    next_question();
}

void RelaxMode::on_pushButton_start_clicked()
{
    next_question();
    ui->stackedWidget->setCurrentIndex(1);
}

void RelaxMode::next_question()
{

    triesLeft = qSettings->tries();
    if(qSettings->cToN() == true && qSettings->nToC() == true && slQuestionsChosen.length() > 0 && slQuestionsNative.length() > 0)
    {
        int rnum = random(1,2);
        if(rnum == 1)
        {
            currentTargetLanguage = qSettings->native();
            currentQuestion = slQuestionsChosen.at(random(0,slQuestionsChosen.length()));
        }
        else
        {
            currentTargetLanguage = qSettings->chosen();
            currentQuestion = slQuestionsNative.at(random(0,slQuestionsNative.length()));
        }
    }
    else if(qSettings->cToN() == true && slQuestionsChosen.length() > 0)
    {
        currentTargetLanguage = qSettings->native();
        currentQuestion = slQuestionsChosen.at(random(0,slQuestionsChosen.length()));
    }
    else if(qSettings->nToC() == true && slQuestionsNative.length() > 0)
    {
        currentTargetLanguage = qSettings->chosen();
        currentQuestion = slQuestionsNative.at(random(0,slQuestionsNative.length()));
    }
    else
    {
        QMessageBox::information(this,"Error","No words available with current settings");
        return;
    }
    ui->label_question->setText(currentQuestion);

    if(qSettings->alternative() == true)
    {
        ui->label_alternative_text->setText(get_alternative_text(currentQuestion));
    }





    inputEnabled = true;
    if(get_answer().length() < 1)
    {
        next_question();
    }
}

QString RelaxMode::get_alternative_text(QString &word)
{
    if(db->open())
    {
        QString sqry = "SELECT DISTINCT word_alternative FROM words WHERE word_text = :word";
        qry->prepare(sqry);
        qry->bindValue(":word", word);

        if(qry->exec())
        {
            if(qry->next())
            {
                return qry->value(0).toString();
            }

        }
        else
        {
            QMessageBox::information(this,"Error", "Error at get_alternative_text: Query failed.");
        }
    }
    else
    {
        QMessageBox::information(this,"Error", "Error at get_alternative_text: Database didn't open.");
    }
    db->close();

    return "";
}


void RelaxMode::on_checkBox_show_alternative_form_toggled(bool checked)
{
    qSettings->setAlternative(checked);
    if(checked == true)
    {
        ui->label_alternative_text->setText(get_alternative_text(currentQuestion));
    }
    else
    {
        ui->label_alternative_text->setText("");
    }

}

void RelaxMode::on_pushButton_end_clicked()
{
    this->close();
}
