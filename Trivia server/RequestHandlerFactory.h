#pragma once

#include "LoginManager.h"
#include "SqliteDataBase.h"
#include "LoginRequestHandler.h"
#include "MenuRequestHandler.h"
#include "RoomManager.h"
#include "StatisticsManager.h"
#include "RoomMemberRequestHandler.h"
#include "RoomAdminRequestHandler.h"
#include "GameRequestHandler.h"

class LoginRequestHandler;
class MenuRequestHandler;
class RoomMemberRequestHandler;
class RoomAdminRequestHandler;
class GameRequestHandler;

/****
 * @brief A factory class to create various request handlers.
 *
 * The RequestHandlerFactory class is responsible for creating instances
 * of different request handlers like LoginRequestHandler, MenuRequestHandler,
 * RoomMemberRequestHandler, RoomAdminRequestHandler, and GameRequestHandler.
 * It also provides access to various managers like LoginManager,
 * StatisticsManager, RoomManager, and GameManager.
 ****/
class RequestHandlerFactory
{
public:
    /****
     * @brief Creates a new LoginRequestHandler.
     *
     * @returns A pointer to a new LoginRequestHandler instance.
     ****/
    LoginRequestHandler* createLoginRequestHandler();

    /****
     * @brief Creates a new MenuRequestHandler.
     *
     * @param user The logged-in user.
     * @returns A pointer to a new MenuRequestHandler instance.
     ****/
    MenuRequestHandler* createMenuRequestHandler(const LoggedUser& user);

    /****
     * @brief Creates a new RoomMemberRequestHandler.
     *
     * @param user The logged-in user.
     * @param room The room to manage.
     * @returns A pointer to a new RoomMemberRequestHandler instance.
     ****/
    RoomMemberRequestHandler* createRoomMemberRequestHandler(const LoggedUser& user, Room* room);

    /****
     * @brief Creates a new RoomAdminRequestHandler.
     *
     * @param user The logged-in user.
     * @param room The room to manage.
     * @returns A pointer to a new RoomAdminRequestHandler instance.
     ****/
    RoomAdminRequestHandler* createRoomAdminRequestHandler(const LoggedUser& user, Room* room);

    /****
     * @brief Creates a new GameRequestHandler.
     *
     * @param game The game instance.
     * @param user The logged-in user.
     * @param answerTimeout The answer timeout value.
     * @returns A pointer to a new GameRequestHandler instance.
     ****/
    GameRequestHandler* createGameRequestHandler(Game* game, const LoggedUser& user, const unsigned int answerTimeout);

    /****
     * @brief Gets the LoginManager instance.
     *
     * @returns A pointer to the LoginManager instance.
     ****/
    LoginManager* getLoginManager();

    /****
     * @brief Gets the StatisticsManager instance.
     *
     * @returns A pointer to the StatisticsManager instance.
     ****/
    StatisticsManager* getStatisticsManager();

    /****
     * @brief Gets the RoomManager instance.
     *
     * @returns A pointer to the RoomManager instance.
     ****/
    RoomManager* getRoomManager();

    /****
     * @brief Gets the GameManager instance.
     *
     * @returns A pointer to the GameManager instance.
     ****/
    GameManager* getGameManager();

    /****
     * @brief Deleted copy constructor to prevent copying.
     ****/
    RequestHandlerFactory(const RequestHandlerFactory& obj) = delete;

    /****
     * @brief Gets the singleton instance of RequestHandlerFactory.
     *
     * @returns A pointer to the singleton instance of RequestHandlerFactory.
     ****/
    static RequestHandlerFactory* getInstance();

private:
    /****
     * @brief Private constructor to enforce singleton pattern.
     ****/
    RequestHandlerFactory();

    static RequestHandlerFactory* instancePtr;

    LoginManager* mLoginManager;
    RoomManager* mRoomManager;
    StatisticsManager* mStatsticsManager;
    GameManager* mGameManager;
    IDataBase* mDb;
};
