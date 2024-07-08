#pragma once

#include "Question.h"
#include "LoggedUser.h"
#include <vector>
#include <map>
#include "SqliteDataBase.h"
#include "CommunicationStructs.h"

using std::map;


class Game
{
public:
	/**
	* @brief Constructs a new Game instance.
	* @param questions A constant reference to a vector containing the questions for the game.
	* @param gameId An unsigned integer representing the unique ID for the game.
	*
	* This constructor initializes a new game instance with the provided questions and assigns a unique game ID.
	*/
	Game(const vector<Question>& questions, const unsigned int gameId);

	/**
	* @brief Retrieves a reference to the current question for a specific user.
	* @param user A const reference to a LoggedUser object representing the player.
	* @throws std::exception If the user has answered all questions.
	*
	* This method retrieves the current question for the provided user based on their internal progress tracker (`currentQuestion`).
	* It throws an exception if the user has already answered all questions in the game.
	*/
	Question& getQuestionForUser(const LoggedUser& user);

	/**
	* @brief Processes an answer submission for a user, updates statistics, and returns the correct answer ID.
	* @param id The answer ID submitted by the user.
	* @param user A const reference to a LoggedUser object representing the player.
	* @param answerTime The time taken by the user to answer the question (in milliseconds).
	* @return The ID of the correct answer for the current question.
	*
	* This method processes an answer submission for a user. It updates the user's statistics (correct/wrong answers, average answer time) based on the submitted answer and time taken.
	* It then checks if the answer is correct and updates the user's progress (`currentQuestion`). The correct answer ID for the current question is returned.
	* If all questions have been answered for the user, game statistics are submitted to the database using `submitGameStatsToDB`.
	*/
	int submitAnswer(const unsigned int id, const LoggedUser& user, const unsigned int answerTime);

	/**
	* @brief Removes a player from the game (marks them as retired).
	* @param user A const reference to a LoggedUser object representing the player.
	*
	* This method removes a player from the game by setting a retired flag (`HasRetired`) in their associated game data within the `mPlayers` map.
	*/
	void removePlayer(const LoggedUser& user);

	/**
	* @brief Adds a player to the game and initializes their game data.
	* @param user A const reference to a LoggedUser object representing the player.
	*
	* This method adds a player to the game by creating an entry in the `mPlayers` map with the user's information and initializing their game data object (`GameData`).
	*/
	void addPlayer(const LoggedUser& user);

	unsigned int getGameId() const; //getter

	/**
	* @brief Generates a vector containing player results (username, correct answers, wrong answers, average answer time, retired flag).
	* @return A vector of `PlayerResults` objects for each player in the game.
	*
	* This method iterates through the `mPlayers` map and creates a `PlayerResults` object for each player containing their username, game statistics, and retired flag.
	* These objects are stored in a vector and returned.
	*/
	vector<PlayerResults> getResults();

	/**
	* @brief Checks if the game is finished (all players retired or answered all questions).
	* @return True if the game is finished, false otherwise.
	*
	* This method iterates through the `mPlayers` map and checks if all players are retired or have answered all questions. If both conditions are true for all players, the game is considered finished and true is returned. Otherwise, false is returned.
	*/
	bool isFinished() const;

	/**
	* @brief Checks if all players are on the same question and can proceed to the next one.
	* @return True if all active players are on the same question, false otherwise.
	*
	* This method iterates through the `mPlayers` map and checks if all non-retired players are on the same question (based on their `currentQuestion` value). If all active players are on the same question, true is returned, indicating it's safe to proceed to the next question. Otherwise, false is returned.
	*/
	bool nextQuestion();

	/**
	* @brief Compares two Game objects for equality based on their game ID.
	* @param other A const reference to another Game object.
	* @return True if both games have the same game ID, false otherwise.
	*
	* This overloaded comparison operator (`==`) checks if two `Game` objects have the same game ID (`mGameId`). If the IDs are equal, true is returned, indicating the games are the same. Otherwise, false is returned.
	*/
	bool operator==(const Game& other);
private:
	vector<Question> mQuestions; //Game's questions
	map<LoggedUser, GameData> mPlayers; //Current players' states
	unsigned int mGameId;
	IDataBase* mDataBase; //DB handler instance

	/**
	* @brief Submits game statistics for a user to the database using the SqliteDataBase object.
	* @param gameData A const reference to the GameData object containing the user's game statistics.
	* @param user A const reference to a LoggedUser object representing the player.
	*
	* This private helper function is used internally by `submitAnswer` when all questions have been answered for a user.
	* It retrieves a pointer to the `SqliteDataBase` object (`mDataBase`) and calls its `submitGameStatistics` function to submit the user's game data (`gameData`) along with the username and game ID.
	*/
	void submitGameStatsToDB(const GameData& gameData, const LoggedUser& user);
};