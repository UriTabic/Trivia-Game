#pragma once

#include <iostream>
#include "IDatabase.h"
#include "sqlite3.h"

class SqliteDataBase : public IDataBase {
public:
	~SqliteDataBase();

	/**
	* Checks if a username exists in the database.
	*
	* @param username The username to check for existence.
	* @return True if the username exists in the database, false otherwise.
	*/
	virtual const bool doesUserExists(const std::string& username) override;

	/**
	* Checks if a username and password combination exist in the database.
	*
	* @param username The username to check.
	* @param password The password to check against.
	* @return True if the username and password match an entry in the database, false otherwise.
	*/
	virtual const bool doesPasswordMatch(const std::string& username, const std::string& password) override;

	/**
	* Adds a new user to the database.
	*
	* @param username The username for the new user.
	* @param password The password for the new user.
	* @param mail The email address for the new user.
	* @return True if the user was successfully added to the database, false otherwise.
	*/
	virtual bool addUser(const std::string& username, const std::string& password, const std::string& mail) override;

	/**
	* Retrieves a list of questions from the database.
	*
	* @param count The number of questions to retrieve.
	* @return A list of Question objects containing the retrieved questions.
	*/
	virtual list<Question> getQuestions(const int count) override;

	/**
	* Gets the average answer time for a specific user (in seconds).
	* -1.0 is returned if there are no recorded answers for the user.
	*
	* @param username The username for which to retrieve the average answer time.
	* @return The average time the user takes to answer questions, or -1.0 if there are no recorded answers.
	*/
	virtual float getPlayerAverageAnswerTime(const string& username) override;

	/**
	* Gets the number of correct answers for a specific user.
	* 0 is returned if there are no recorded answers for the user.
	*
	* @param username The username for which to retrieve the number of correct answers.
	* @return The number of correct answers the user has recorded.
	*/
	virtual int getNumOfCorrectAnswers(const string& username) override;

	/**
	* Gets the total number of answers (correct or incorrect) for a specific user.
	* 0 is returned if there are no recorded answers for the user.
	*
	* @param username The username for which to retrieve the total number of answers.
	* @return The total number of answers the user has recorded.
	*/
	virtual int getNumOfTotalAnswers(const string& username) override;

	/**
	* Gets the number of games played by a specific user.
	* 0 is returned if there are no recorded games for the user.
	*
	* @param username The username for which to retrieve the number of games played.
	* @return The number of games the user has played.
	*/
	virtual int getNumOfPlayerGames(const string& username) override;

	/**
	* Gets the current score for a specific user.
	* -1 is returned if there is no recorded score for the user.
	*
	* @param username The username for which to retrieve the score.
	* @return The user's score.
	*/
	virtual int getPlayerScore(const std::string& username) override;

	/**
	* Retrieves a list of usernames with the highest scores.
	*
	* @return A vector of strings containing usernames with the highest scores.
	*/
	virtual vector<string> getHighScores() override;

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
	virtual void submitGameStatistics(const GameData& data, const string& username, const int gameID) override;

	/**
	* @brief Gets next game ID from databas.
	* @return Next game ID or -1 on error.
	*/
	virtual int getNextId() override;

	/**
	* Opens a connection to the database.
	*
	* @return True if the connection is successfully opened, false otherwise.
	*/
	virtual bool open() override;

	/**
	* Closes the connection to the database.
	*/
	virtual void close() override;	/**
	* Clears any cached data or temporary state within the database interface.
	*/
	virtual void clear() override;

	/**
	* Provides a static method to retrieve the single instance of SqliteDataBase
	* following the Singleton design pattern.
	*
	* @return A pointer to the single instance of SqliteDataBase.
	*/
	static SqliteDataBase* getInstance();
	SqliteDataBase(const SqliteDataBase& obj) = delete;

private:
	SqliteDataBase();  // Private constructor, enforces singleton pattern

	/**
	* Callback function used by sqlite3_exec for boolean results (e.g., EXISTS queries).
	* Sets the provided data pointer (assumed to be a bool) to the retrieved value.
	*
	* @param data A pointer to the data to be populated with the result.
	* @param argc The number of columns returned by the query.
	* @param argv An array of strings containing the values of the results.
	* @param azColName An array of strings containing the names of the columns.
	* @return Always 0.
	*/
	static int callback_getBool(void* data, int argc, char** argv, char** azColName);

	/**
	* Callback function used by sqlite3_exec for integer results.
	* Sets the provided data pointer (assumed to be an int) to the retrieved integer value.
	*
	* @param data A pointer to the data to be populated with the result.
	* @param argc The number of columns returned by the query.
	* @param argv An array of strings containing the values of the results.
	* @param azColName An array of strings containing the names of the columns.
	* @return Always 0.
	*/
	static int callback_getInt(void* data, int argc, char** argv, char** azColName);

	/**
	* Callback function used by sqlite3_exec to populate a list of Question objects from a query.
	*
	* @param data A pointer to the list<Question> to be populated with results.
	* @param argc The number of columns returned by the query.
	* @param argv An array of strings containing the values of the results.
	* @param azColName An array of strings containing the names of the columns.
	* @return Always 0.
	*/
	static int callback_fill_questions(void* data, int argc, char** argv, char** azColName);

	/**
	* Callback function used by sqlite3_exec to populate a map of usernames and their scores from a query.
	*
	* @param data A pointer to the std::map<string, int> to be populated with results.
	* @param argc The number of columns returned by the query.
	* @param argv An array of strings containing the values of the results.
	* @param azColName An array of strings containing the names of the columns.
	* @return Always 0.
	*/
	static int callback_fill_scores(void* data, int argc, char** argv, char** azColName);

	/**
	* Initializes empty statistics for a new user in the database.
	*
	* @param username The username for which to initialize empty stats.
	*/
	void initEmptyStats(const std::string& username);

	/**
	* Retrieves the total answer time for a user from the database (in seconds).
	*
	* @param username The username for which to retrieve the total answer time.
	* @return The total time the user has spent answering questions, or 0 if there are no recorded answers.
	*/
	int getPlayerTotalAnswerTime(const string& username);
	/**
	* Executes an SQLite query with a provided callback function and data pointer.
	*
	* @param query The SQL query string to execute.
	* @param callback A pointer to the callback function to be invoked for each row of results.
	* @param data A pointer to data to be passed to the callback function.
	* @return The integer result code from the sqlite3_exec call.
	*/
	int execute(const std::string& query, int(*callback)(void*, int, char**, char**) = nullptr, void* data = nullptr);


	sqlite3* _db; //Data base instance
	std::string _dbFileName = "triviaDB.sqlite";

	static SqliteDataBase* instancePtr; //Singelton instance

};