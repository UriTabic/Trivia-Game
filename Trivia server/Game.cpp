#include "Game.h"

Game::Game(const vector<Question>& questions, const unsigned int gameId)
{
	mQuestions = questions;
	mGameId = gameId;
	mDataBase = SqliteDataBase::getInstance();
}

Question& Game::getQuestionForUser(const LoggedUser& user)
{
	if(mPlayers.at(user).currentQuestion < mQuestions.size())
		return mQuestions.at(mPlayers.at(user).currentQuestion);
	throw std::exception("out of questions");
}

int Game::submitAnswer(const unsigned int id, const LoggedUser& user, const unsigned int answerTime)
{
	//Calculate time avg.
	mPlayers.at(user).averangeAnswerTime *= mPlayers.at(user).currentQuestion / (double)(mPlayers.at(user).currentQuestion + 1);
	int n = answerTime / (mPlayers.at(user).currentQuestion + 1);
	mPlayers.at(user).averangeAnswerTime += n;

	bool isCorrect = false;
	
	//Check if user was right or wrong and increment the respectively variable.
	if (mQuestions.at(mPlayers.at(user).currentQuestion).getCorrectAnswerId() == id)
	{
		mPlayers.at(user).correctAnswerCount++;
		isCorrect = true;
	}
	else
	{
		mPlayers.at(user).wrongAnswerCount++;
	}

	//If the game is over saves the user's scores.
	if (mPlayers.at(user).currentQuestion >= mQuestions.size() - 1)
	{
		submitGameStatsToDB(mPlayers.at(user), user);
	}
	return mQuestions[mPlayers.at(user).currentQuestion++].getCorrectAnswerId();
}

void Game::removePlayer(const LoggedUser& user)
{
	mPlayers[user].HasRetired = 1;
}

void Game::addPlayer(const LoggedUser& user)
{
	mPlayers[user] = GameData();
}

unsigned int Game::getGameId() const
{
	return mGameId;
}

vector<PlayerResults> Game::getResults()
{
	vector<PlayerResults> results;
	for (const auto& it : mPlayers)
	{
		results.push_back(PlayerResults{
				it.first.getUsername(),
				it.second.correctAnswerCount,
				it.second.wrongAnswerCount,
				it.second.averangeAnswerTime,
				it.second.HasRetired
			});
	}
	return results;
}

bool Game::isFinished() const
{
	for (auto& it : mPlayers)
	{
		//If user is still playing and still have questions to answer returns false.
		if (!it.second.HasRetired && it.second.currentQuestion < mQuestions.size()) 
			return false;
	}
	return true;
}

bool Game::nextQuestion()
{
	if (mPlayers.size() < 1) return true;
	int currQuestion = 0;
	for (auto it = mPlayers.begin(); it != mPlayers.end(); it++)
	{
		currQuestion = it->second.currentQuestion;
		if (!it->second.HasRetired) 
		{
			break;
		}
	}
	for (auto& it : mPlayers)
	{
		if (it.second.HasRetired) continue;
		if (it.second.currentQuestion != currQuestion) return false;
	}
	return true;
}

bool Game::operator==(const Game& other)
{
	return this->mGameId == other.mGameId;
}

void Game::submitGameStatsToDB(const GameData& gameData, const LoggedUser& user)
{
	mDataBase->submitGameStatistics(gameData, user.getUsername(), mGameId);
}
