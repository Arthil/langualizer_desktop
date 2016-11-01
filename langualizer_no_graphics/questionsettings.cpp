#include "questionsettings.h"

QuestionSettings::QuestionSettings()
{
    //Default settings

    wordTypes.adj = true;
    wordTypes.adv = true;
    wordTypes.noun = true;
    wordTypes.other = true;
    wordTypes.prep = true;
    wordTypes.pron = true;
    wordTypes.verb = true;

    nativeToChosen = true;
    chosenToNative = true;
    showTips = true;
    allWords = true;
    showAlternativeText = true;

    numTries = 1;

}

QuestionSettings::~QuestionSettings()
{
}
