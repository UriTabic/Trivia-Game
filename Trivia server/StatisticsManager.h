#pragma once

#include "SqliteDataBase.h"
#include <vector>
#include <string>

using std::vector;
using std::string;

/****
 * @brief The StatisticsManager class manages the retrieval of statistical data.
 *
 * This singleton class provides methods to get high scores and user-specific statistics
 * by interacting with the database.
 ****/
class StatisticsManager
{
public:
    /****
     * @brief Retrieves the high scores from the database.
     *
     * This method fetches the high scores from the database.
     *
     * @returns A vector of strings containing high scores.
     ****/
    vector<string> getHighScore();

    /****
     * @brief Retrieves the statistics for a specific user.
     *
     * This method fetches and formats the statistics for the specified user.
     *
     * @param username The username of the user whose statistics are to be retrieved.
     * @returns A vector of strings containing the user's statistics.
     ****/
    vector<string> getUserStatistics(const string& username);

    /****
     * @brief Deleted copy constructor to enforce singleton pattern.
     *
     * This prevents copying of the singleton instance.
     *
     * @param obj A reference to another StatisticsManager object.
     ****/
    StatisticsManager(const StatisticsManager& obj) = delete;

    /****
     * @brief Gets the singleton instance of StatisticsManager.
     *
     * This method ensures that only one instance of StatisticsManager exists.
     *
     * @returns A pointer to the singleton instance of StatisticsManager.
     ****/
    static StatisticsManager* getInstance();

private:
    /****
     * @brief Private constructor to enforce singleton pattern.
     *
     * This constructor initializes necessary members.
     ****/
    StatisticsManager();

    IDataBase* mDb; ///< Pointer to the database instance.
    static StatisticsManager* instancePtr; ///< Pointer to the singleton instance.
};
