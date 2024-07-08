#include "RoomAdminRequestHandler.h"
#include "JsonResponsePacketSerializer.h"

RoomAdminRequestHandler::RoomAdminRequestHandler(Room* room, const LoggedUser& user)
{
	mRoom = room;
	mUser = user;
	mRoomManager = RoomManager::getInstance();
	mFactory = RequestHandlerFactory::getInstance();
}

bool RoomAdminRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case CLOSE_ROOM_REQUEST:
	case GET_ROOM_STATE_REQUEST:
	case START_GAME_REQUEST:
		return true;
	default:
		return false;
	};
}

RequestResult RoomAdminRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case CLOSE_ROOM_REQUEST:
		return closeRoom(reqInfo);
	case GET_ROOM_STATE_REQUEST:
		return getRoomState(reqInfo);
	case START_GAME_REQUEST:
		return startGame(reqInfo);
	}
	return RequestResult();
}

RequestResult RoomAdminRequestHandler::startGame(const RequestInfo request)
{
	RequestResult result;
	mRoom->setState(RoomState::STARTED);
	StartGameResponse response;
	response.status = SUCCESS;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = mFactory->createGameRequestHandler(&(GameManager::getInstance()->createGame(mRoom->getRoomData())), mUser, mRoom->getRoomData().timePerQuestion);
	return result;
}

RequestResult RoomAdminRequestHandler::closeRoom(const RequestInfo request)
{
	RequestResult result;
	mRoomManager->deleteRoom(mRoom->getRoomData().id);
	mRoom->getRoomData().state = RoomState::CLOSED;
	CloseRoomResponse response;
	response.status = SUCCESS;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = mFactory->createMenuRequestHandler(mUser);
	return result;
}

RequestResult RoomAdminRequestHandler::getRoomState(const RequestInfo request)
{
	RequestResult result;
	GetRoomStateResponse response;
	response.status = SUCCESS;
	RoomData roomData = mRoom->getRoomData();
	response.answerTimeout = roomData.timePerQuestion;
	response.questionCount = roomData.numOfQuestionsInGame;
	response.state = roomData.state;
	response.players = mRoom->getAllUsers();
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = this;
	return result;
}
