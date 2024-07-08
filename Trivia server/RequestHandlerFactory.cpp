#include "RequestHandlerFactory.h"

RequestHandlerFactory* RequestHandlerFactory::instancePtr = nullptr;


RequestHandlerFactory::RequestHandlerFactory()
{
	mLoginManager = LoginManager::getInstance();
	mStatsticsManager = StatisticsManager::getInstance();
	mRoomManager = RoomManager::getInstance();
	mGameManager = GameManager::getInstance();
	mDb = SqliteDataBase::getInstance();
}

LoginRequestHandler* RequestHandlerFactory::createLoginRequestHandler()
{
    return new LoginRequestHandler();
}

MenuRequestHandler* RequestHandlerFactory::createMenuRequestHandler(const LoggedUser& user)
{
    return new MenuRequestHandler(user);
}

RoomMemberRequestHandler* RequestHandlerFactory::createRoomMemberRequestHandler(const LoggedUser& user, Room* room)
{
	return new RoomMemberRequestHandler(room, user);
}

RoomAdminRequestHandler* RequestHandlerFactory::createRoomAdminRequestHandler(const LoggedUser& user, Room* room)
{
	return new RoomAdminRequestHandler(room, user);
}

GameRequestHandler* RequestHandlerFactory::createGameRequestHandler(Game* game, const LoggedUser& user, const unsigned int answerTimeout)
{
	return new GameRequestHandler(game, user, answerTimeout);
}

LoginManager* RequestHandlerFactory::getLoginManager()
{
    return LoginManager::getInstance();
}

StatisticsManager* RequestHandlerFactory::getStatisticsManager()
{
	return StatisticsManager::getInstance();
}

RoomManager* RequestHandlerFactory::getRoomManager()
{
	return RoomManager::getInstance();
}

GameManager* RequestHandlerFactory::getGameManager()
{
	return GameManager::getInstance();
}

RequestHandlerFactory* RequestHandlerFactory::getInstance()
{
	if (instancePtr == nullptr)
	{
		instancePtr = new RequestHandlerFactory();
	}
	return instancePtr;
}
