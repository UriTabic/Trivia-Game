#include "Question.h"

Question::Question(const string& question, const vector<string>& possibleAnswers, const int correctAnswerId)
{
    mQuestion = question;
    mPossibleAnswers = possibleAnswers;
    mCorrectAnswerId = correctAnswerId;
}

string Question::getQuestion() const
{
    return mQuestion;
}

vector<string> Question::getPossibleAnswers() const
{
    return mPossibleAnswers;
}

int Question::getCorrectAnswerId() const
{
    return mCorrectAnswerId;
}
