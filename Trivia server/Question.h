#pragma once
#include<vector>
#include<string>
using std::vector;
using std::string;

/*
* A class that describes a question for the game.
*/
class Question
{
public:
	/*
	* Generate an instance with the question, possible answers and the id of the correct one.
	*/
	Question(const string& question, const vector<string>& possibleAnswers, const int correctAnswerId);
	/*
	* @returns the question.
	*/
	string getQuestion() const;
	/*
	* @returns the possible answers.
	*/
	vector<string> getPossibleAnswers() const;
	/*
	* @returns the correct answer id.
	*/
	int getCorrectAnswerId() const;
private:
	string mQuestion;
	vector<string> mPossibleAnswers;
	int mCorrectAnswerId;
};

