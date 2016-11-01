#ifndef QUESTIONSETTINGS_H
#define QUESTIONSETTINGS_H
#include <QString>
#include <QStringList>


class QuestionSettings
{

public:
    QuestionSettings();
    ~QuestionSettings();

private:
    struct types
    {
        bool noun;
        bool verb;
        bool adj;
        bool adv;
        bool pron;
        bool prep;
        bool other;
    };

    types wordTypes;

    bool allWords;

    bool nativeToChosen;
    bool chosenToNative;

    bool showTips;

    int numTries;
    QString native_language;
    QString chosen_language;

    QStringList chosen_chapters;
    QString chosen_book;

    bool showAlternativeText;


public:

    // Getters
    inline const bool noun() const { return wordTypes.noun; }
    inline const bool verb() const { return wordTypes.verb; }
    inline const bool adj() const { return wordTypes.adj; }
    inline const bool adv() const { return wordTypes.adv; }
    inline const bool pron() const { return wordTypes.pron; }
    inline const bool prep() const { return wordTypes.prep; }
    inline const bool other() const { return wordTypes.other; }

    inline const bool all_Words() const { return allWords; }
    inline const bool nToC() const { return nativeToChosen; }
    inline const bool cToN() const { return chosenToNative; }
    inline const bool tips() const { return showTips; }
    inline const int tries() const { return numTries; }

    inline const QString native() const { return native_language; }
    inline const QString chosen() const { return chosen_language; }
    inline const QStringList chapters() const {return chosen_chapters;}
    inline const QString book() const {return chosen_book;}

    inline const bool alternative() const {return showAlternativeText;}

    // Setters
    inline void setNoun(bool b){wordTypes.noun = b;}
    inline void setVerb(bool b){wordTypes.verb = b;}
    inline void setAdj(bool b){wordTypes.adj = b;}
    inline void setAdv(bool b){wordTypes.adv = b;}
    inline void setPron(bool b){wordTypes.pron = b;}
    inline void setPrep(bool b){wordTypes.prep = b;}
    inline void setOther(bool b){wordTypes.other = b;}

    inline void setAllWords(bool b){allWords = b;}
    inline void setNToC(bool b){nativeToChosen = b;}
    inline void setCToN(bool b){chosenToNative = b;}
    inline void setTips(bool b){showTips = b;}
    inline void setTries(int n){numTries = n;}
    inline void setNative(QString &s){native_language = s;}
    inline void setChosen(QString &s){chosen_language = s;}
    inline void setChapters(QStringList &sl){chosen_chapters = sl;}
    inline void setBook(QString &b){chosen_book = b;}
    inline void setAlternative(bool b){showAlternativeText = b;}



};

#endif // QUESTIONSETTINGS_H
