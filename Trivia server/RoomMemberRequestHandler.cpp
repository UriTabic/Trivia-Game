#include "RoomMemberRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "RequestHandlerFactory.h"

RoomMemberRequestHandler::RoomMemberRequestHandler(Room* room, const LoggedUser& user)
{
    mRoom = room;
    mUser = user;
    mRoomManager = RoomManager::getInstance();
    mFactory = RequestHandlerFactory::getInstance();
}

bool RoomMemberRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case LEAVE_ROOM_REQUEST:
	case GET_ROOM_STATE_REQUEST:
		return true;
	default:
		return false;
	}
}

RequestResult RoomMemberRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case LEAVE_ROOM_REQUEST:
		return leaveRoom(reqInfo);
	case GET_ROOM_STATE_REQUEST:
		return getRoomState(reqInfo);
	}
}

RequestResult RoomMemberRequestHandler::leaveRoom(const RequestInfo request)
{
	RequestResult result;
	mRoom->removeUser(mUser);
	LeaveRoomResponse response;
	response.status = SUCCESS;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = mFactory->createMenuRequestHandler(mUser);
    return result;
}

RequestResult RoomMemberRequestHandler::getRoomState(const RequestInfo request)
{
	RequestResult result;
	GetRoomStateResponse response;
	response.status = SUCCESS;
	RoomData roomData;
	try {
		roomData = mRoom->getRoomData();
		response.players = mRoom->getAllUsers();
	}
	catch(std::exception e)
	{
		roomData.state = RoomState::CLOSED;
	}
	response.answerTimeout = roomData.timePerQuestion;
	response.questionCount = roomData.numOfQuestionsInGame;
	response.state = roomData.state;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	IRequestHandler* next = nullptr;
	Game* game = nullptr;
	switch (roomData.state)
	{
	case RoomState::CLOSED:
		next = mFactory->createMenuRequestHandler(mUser);
		break;
	case RoomState::OPENED:
		next = this;
		break;
	case RoomState::STARTED:
		game = &(GameManager::getInstance()->createGame(mRoom->getRoomData()));
		next = mFactory->createGameRequestHandler(game, mUser, mRoom->getRoomData().timePerQuestion);
		break;
	default:
		next = nullptr;
		break;
	}
	result.nextHandler = next;
	return result;
}
