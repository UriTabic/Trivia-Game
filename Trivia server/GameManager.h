#pragma once

#include "Game.h"
#include "Room.h"
#include "IDatabase.h"
#include <thread>
#include <chrono>

/****
 * @brief The GameManager class is responsible for managing game instances.
 *
 * This singleton class handles the creation, deletion, and management of game instances.
 * It also provides a mechanism to periodically remove finished games.
 ****/
class GameManager
{
public:
    /****
     * @brief Deleted copy constructor to enforce singleton pattern.
     *
     * This prevents copying of the singleton instance.
     *
     * @param obj A reference to another GameManager object.
     ****/
    GameManager(const GameManager& obj) = delete;

    /****
     * @brief Gets the singleton instance of GameManager.
     *
     * This method ensures that only one instance of GameManager exists.
     *
     * @returns A pointer to the singleton instance of GameManager.
     ****/
    static GameManager* getInstance();

    /****
     * @brief Creates a new game or returns an existing game based on the provided room data.
     *
     * This method creates a new game if no game with the given room ID exists,
     * otherwise it returns the existing game.
     *
     * @param room A reference to a RoomData object containing the room's data.
     * @returns A reference to the created or existing Game object.
     ****/
    Game& createGame(RoomData& room);

    /****
     * @brief Deletes a game with the specified game ID.
     *
     * This method removes a game from the game list based on its ID.
     *
     * @param gameId An unsigned int representing the ID of the game to be deleted.
     ****/
    void deleteGame(const unsigned int gameId);

    /****
     * @brief Starts a separate thread to remove finished games.
     *
     * This method spawns a new thread that continuously checks for finished games and removes them.
     *
     * @returns A std::thread object running the removeFinishedGames method.
     ****/
    std::thread startRemoveFinishedGames();

private:
    /****
     * @brief Periodically removes finished games from the game list.
     *
     * This method continuously checks the game list and removes games that have finished.
     ****/
    void removeFinishedGames();

    /****
     * @brief Private constructor to enforce singleton pattern.
     *
     * This constructor initializes the database connection and other necessary members.
     ****/
    GameManager();

    /****
     * @brief Destructor for GameManager.
     *
     * This destructor cleans up the singleton instance.
     ****/
    ~GameManager();

    IDataBase* mDataBase;             ///< Pointer to the database instance.
    vector<Game> mGames;              ///< List of active games.
    static GameManager* instancePtr;  ///< Pointer to the singleton instance.
};