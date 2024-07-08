#pragma once

#include "Question.h"
#include <list>
using std::list;

struct GameData
{
	unsigned int currentQuestion;
	unsigned int correctAnswerCount;
	unsigned int wrongAnswerCount;
	unsigned int averangeAnswerTime;
	unsigned int HasRetired;
};

/*
* An interface for accessing the DB.
*/
class IDataBase
{
public:
	virtual ~IDataBase() = default;

	/*
	* Checks if the username exists in the database.
	*
	* @param username The username to check for existence.
	* @return True if the username exists in the database, false otherwise.
	*/
	virtual const bool doesUserExists(const std::string& username) = 0;

	/*
	* Checks if there is a username with the matching password in the database.
	*
	* @param username The username to check.
	* @param password The password to check against.
	* @return True if the username and password match an entry in the database, false otherwise.
	*/
	virtual const bool doesPasswordMatch(const std::string& username, const std::string& password) = 0;

	/*
	* Adds a new user to the database.
	*
	* @param username The username for the new user.
	* @param password The password for the new user.
	* @param mail The email address for the new user.
	* @return True if the user was successfully added to the database, false otherwise.
	*/
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& mail) = 0;

	/*
	* Retrieves a list of questions from the database.
	*
	* @param count The number of questions to retrieve.
	* @return A list of Question objects containing the retrieved questions.
	*/
	virtual list<Question> getQuestions(const int count) = 0;

	/*
	* Gets the average answer time for a specific user.
	*
	* @param username The username for which to retrieve the average answer time.
	* @return The average time (in seconds) the user takes to answer questions,
	*         or -1.0 if there are no recorded answers for the user.
	*/
	virtual float getPlayerAverageAnswerTime(const string& username) = 0;

	/*
	* Gets the number of correct answers for a specific user.
	*
	* @param username The username for which to retrieve the number of correct answers.
	* @return The number of correct answers the user has recorded, or 0 if there are no recorded answers.
	*/
	virtual int getNumOfCorrectAnswers(const string& username) = 0;

	/*
	* Gets the total number of answers (correct or incorrect) for a specific user.
	*
	* @param username The username for which to retrieve the total number of answers.
	* @return The total number of answers the user has recorded, or 0 if there are no recorded answers.
	*/
	virtual int getNumOfTotalAnswers(const string& username) = 0;

	/*
	* Gets the number of games played by a specific user.
	*
	* @param username The username for which to retrieve the number of games played.
	* @return The number of games the user has played, or 0 if there are no recorded games.
	*/
	virtual int getNumOfPlayerGames(const string& username) = 0;

	/*
	* Gets the current score for a specific user.
	*
	* @param username The username for which to retrieve the score.
	* @return The user's score, or -1 if there is no recorded score for the user.
	*/
	virtual int getPlayerScore(const string& username) = 0;

	/*
	* Retrieves a list of usernames with the highest scores.
	*
	* @return A vector of strings containing usernames with the highest scores.
	*/
	virtual vector<string> getHighScores() = 0;

	/**
	 * @brief Submits game statistics for a user to the database.
	 *
	 * This method inserts a new record into the USERS_GAMES table containing
	 * the provided game statistics for a user.
	 *
	 * @param data A const reference to a GameData object containing the game statistics.
	 * @param username A const string reference containing the username of the player.
	 * @param gameId An integer representing the ID of the game played.
	 *
	 * @return None
	 */
	virtual void submitGameStatistics(const GameData& data, const string& username, const int gameId) = 0;

	/**
	* @brief Gets next game ID from databas.
	* @return Next game ID or -1 on error.
	*/
	virtual int getNextId() = 0;

	/*
	* Opens a connection to the database.
	*
	* @return True if the connection is successfully opened, false otherwise.
	*/
	virtual bool open() = 0;

	/*
	* Closes the connection to the database.
	*/
	virtual void close() = 0;

	/*
	* Clears any cached data or temporary state within the database interface.
	*/
	virtual void clear() = 0;

private:
	/*
	* Initializes empty statistics for a new user (called internally).
	*/
	virtual void initEmptyStats(const std::string& username) = 0;
};