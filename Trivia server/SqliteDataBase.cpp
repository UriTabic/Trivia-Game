#include "SqliteDataBase.h"
#include <algorithm>
#include <map>

SqliteDataBase* SqliteDataBase::instancePtr = nullptr; //Assign static veriable init value.
using std::to_string;

SqliteDataBase::~SqliteDataBase()
{
	clear();
	close();
	delete(instancePtr);
	instancePtr = nullptr;
}

const bool SqliteDataBase::doesUserExists(const std::string& username)
{
	bool result = false;

	std::string query = "SELECT EXISTS (SELECT 1 FROM USERS WHERE USERNAME = '" + username + "');";

	execute(query, callback_getBool, &result);

	return result;
}

const bool SqliteDataBase::doesPasswordMatch(const std::string& username, const std::string& password)
{
	bool result = false;

	std::string query = "SELECT EXISTS (SELECT 1 FROM USERS WHERE USERNAME = '" + username + "' AND PASSWORD = '" + password + "');";

	execute(query, callback_getBool, &result);

	return result;
}

bool SqliteDataBase::addUser(const std::string& username, const std::string& password, const std::string& mail)
{
	std::string query = "INSERT INTO USERS (USERNAME, PASSWORD, MAIL) VALUES ('" + username + "', '" + password + "', '" + mail + "');";

	int check = execute(query);
	return check == SQLITE_OK;
}

list<Question> SqliteDataBase::getQuestions(const int count)
{
	string query = "SELECT * FROM QUESTIONS ORDER BY RANDOM() LIMIT " + to_string(count) + "; ";
	list<Question> questions;
	execute(query, callback_fill_questions, &questions);
	return questions;
}

float SqliteDataBase::getPlayerAverageAnswerTime(const string& username)
{
	string query = "SELECT AVG(TIME_AVG / 10.0) AS average FROM USERS_GAMES WHERE USERNAME = '" + username + "';";
	int avg = -1;
	execute(query, callback_getInt, &avg);
	return avg;
}

int SqliteDataBase::getNumOfCorrectAnswers(const string& username)
{
	string query = "SELECT SUM(CORRECT_ANSWERS) AS COUNT FROM USERS_GAMES WHERE USERNAME = '" + username + "';";
	int count;
	execute(query, callback_getInt, &count);
	return count;
}

int SqliteDataBase::getNumOfTotalAnswers(const string& username)
{
	string query = "SELECT (SUM(WRONG_ANSWERS) + SUM(CORRECT_ANSWERS)) AS COUNT FROM USERS_GAMES WHERE USERNAME = '" + username + "';";
	int count;
	execute(query, callback_getInt, &count);
	return count;
}

int SqliteDataBase::getNumOfPlayerGames(const string& username)
{
	string query = "SELECT COUNT(DISTINCT GAME_ID) AS COUNT FROM USERS_GAMES WHERE USERNAME = '" + username + "';";
	int count;
	execute(query, callback_getInt, &count);
	return count;
}

int SqliteDataBase::getPlayerScore(const string& username)
{
	string query = "SELECT AVG((CAST(CORRECT_ANSWERS AS FLOAT) / CAST(WRONG_ANSWERS + CORRECT_ANSWERS AS FLOAT)) * (10000.0 / TIME_AVG)) AS avg_score, USERNAME FROM USERS_GAMES WHERE USERNAME = '" + username + "' GROUP BY USERNAME;";
	int count;
	execute(query, callback_getInt, &count);
	return count;
}

vector<string> SqliteDataBase::getHighScores()
{
	string query = "SELECT USERNAME, (CAST(CORRECT_ANSWERS AS FLOAT) / CAST(WRONG_ANSWERS + CORRECT_ANSWERS AS FLOAT)) * (10000.0 / TIME_AVG) AS score FROM USERS_GAMES ORDER BY score DESC LIMIT 5;";
	vector < std::pair<string, int>> scores;
	execute(query, callback_fill_scores, &scores);
	vector<string> result;
	int i = 0;
	for (auto it = scores.begin(); it != scores.end(); it++)
	{
		result.push_back(to_string(++i) + ": " + it->first + " - " + to_string(it->second));
	}
	return result;
}


void SqliteDataBase::submitGameStatistics(const GameData& data, const string& username, const int gameId)
{
	string query = "INSERT INTO USERS_GAMES (USERNAME, GAME_ID, CORRECT_ANSWERS, WRONG_ANSWERS, TIME_AVG) VALUES ('" + username + "', " + to_string(gameId) + ", " + to_string(data.correctAnswerCount) + ", " + to_string(data.wrongAnswerCount) + ", " + to_string(data.averangeAnswerTime) + ");";
	execute(query);
}

int SqliteDataBase::getNextId()
{
	int id = -1;

	execute("SELECT MAX(GAME_ID) FROM USERS_GAMES", callback_getInt, &id);

	return id + 1;
}

bool SqliteDataBase::open()
{
	int res = sqlite3_open(_dbFileName.c_str(), &_db);
	if (res == SQLITE_OK)
	{
		return true;
	}
	else
	{
		_db = nullptr;
		std::cout << "Failed to open DB" << std::endl;
		return false;
	}
}

void SqliteDataBase::close()
{
	sqlite3_close(_db);
	_db = nullptr;
}

void SqliteDataBase::clear()
{
}

SqliteDataBase* SqliteDataBase::getInstance()
{
	{
		if (instancePtr == nullptr)
		{
			instancePtr = new SqliteDataBase();
		}
		return instancePtr;
	}
}

SqliteDataBase::SqliteDataBase()
{
	open();
}

int SqliteDataBase::callback_getBool(void* data, int argc, char** argv, char** azColName)
{
	for (int i = 0; i < argc; i++)
	{
		*((bool*)data) = atoi(argv[i]); // Convert the result to a boolean and assign it to data.
	}

	return 0;
}

int SqliteDataBase::callback_getInt(void* data, int argc, char** argv, char** azColName)
{
	if(*argv != NULL)
	{
		*((int*)data) = atoi(argv[0]); // Convert the result to an integer and assign it to data.
	}
	else
	{
		*((int*)data) = 0; // If no result is found, assign 0 to data.
	}
	return 0;
}

int SqliteDataBase::callback_fill_questions(void* data, int argc, char** argv, char** azColName)
{
	list<Question>* questions = (list<Question>*)data;
	string question = "";
	string answer = "";
	vector<string> possibleAnswers;
	int correctAnswerId;
	for (int i = 0; i < argc; i++) {
		if (string(azColName[i]) == "QUESTION") {
			question = argv[i];
		}
		else if (string(azColName[i]) == "CORRECT") {
			answer = argv[i];
		}
		else if (string(azColName[i]).rfind("WRONG", 0) == 0) { // Check if the column name starts with "WRONG"
			possibleAnswers.push_back(argv[i]);
		}
	}
	possibleAnswers.push_back(answer);
	std::sort(possibleAnswers.begin(), possibleAnswers.end());
	auto it = std::find(possibleAnswers.begin(), possibleAnswers.end(), answer);
	correctAnswerId = std::distance(possibleAnswers.begin(), it);
	questions->push_back(Question(question, possibleAnswers, correctAnswerId));
	return 0;
}

int SqliteDataBase::callback_fill_scores(void* data, int argc, char** argv, char** azColName)
{
	std::pair<string, int> score;
	score.first = argv[0];
	score.second = atoi(argv[1]);
	((std::vector<std::pair<string, int>>*)data)->push_back(score);
	return 0;
}

void SqliteDataBase::initEmptyStats(const std::string& username)
{
	std::string query = "INSERT INTO ANSWERS (USERNAME, GAME_COUNT, TOTAL_ANSWERS, TOTAL_CORECT_ANSWERS, TOTAL_TIME) VALUES ('" + username + "', 0, 0, 0, 0);";
	execute(query);
}

int SqliteDataBase::getPlayerTotalAnswerTime(const string& username)
{
	string query = "SELECT TOTAL_TIME FROM ANSWERS WHERE USERNAME = '" + username + "';";
	int total = 0;
	execute(query, callback_getInt, &total);
	return total;
}

int SqliteDataBase::execute(const std::string& query, int(*callback)(void*, int, char**, char**), void* data)
{
	char* errMessage = nullptr;

	int res = sqlite3_exec(_db, query.c_str(), callback, data, &errMessage);
	if (res != SQLITE_OK)
	{
		std::cerr << "error running query:" << std::endl;
		std::cerr << errMessage << std::endl;
	}
	return res;
}
