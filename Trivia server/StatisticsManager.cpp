#include "StatisticsManager.h"
using std::to_string;

StatisticsManager* StatisticsManager::instancePtr = nullptr;

StatisticsManager::StatisticsManager()
{
	mDb = SqliteDataBase::getInstance();	
}

vector<string> StatisticsManager::getHighScore()
{
	return mDb->getHighScores();
}

vector<string> StatisticsManager::getUserStatistics(const string& username)
{
	vector<string> data;

	data.push_back(username + ":");
	int gameCount = mDb->getNumOfPlayerGames(username);
	if(gameCount > 0)
	{
		data.push_back("Game count: " + to_string(gameCount));
		data.push_back("Avarage answer time: " + to_string(mDb->getPlayerAverageAnswerTime(username)));
		data.push_back("Correct answers: " + to_string(mDb->getNumOfCorrectAnswers(username)));
		data.push_back("Total answers: " + to_string(mDb->getNumOfTotalAnswers(username)));
		data.push_back("Average score: " + to_string(mDb->getPlayerScore(username)));
	}
	else
	{
		data.push_back("No games yet");
	}

	return data;
}

StatisticsManager* StatisticsManager::getInstance()
{
	if (instancePtr == nullptr)
	{
		instancePtr = new StatisticsManager();
	}
	return instancePtr;
}
