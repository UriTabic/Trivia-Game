#include "MenuRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
bool MenuRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case LOGOUT_REQUEST:
	case GET_PLAYERS_IN_ROOM_REQUEST:
	case JOIN_ROOM_REQUEST:
	case CREATE_ROOM_REQUEST:
	case GET_ROOMS_REQUEST:
	case GET_HIGH_SCORE_REQUEST:
	case GET_PERSONAL_STATS_REQUEST:
		return true;
	default:
		return false;
	}
}

RequestResult MenuRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case LOGOUT_REQUEST:
		return signout(reqInfo);
	case GET_PLAYERS_IN_ROOM_REQUEST:
		return getPlayersInRoom(reqInfo);
	case JOIN_ROOM_REQUEST:
		return joinRoom(reqInfo);
	case CREATE_ROOM_REQUEST:
		return createRoom(reqInfo);
	case GET_ROOMS_REQUEST:
		return getRooms(reqInfo);
	case GET_HIGH_SCORE_REQUEST:
		return getHighScore(reqInfo);
	case GET_PERSONAL_STATS_REQUEST:
		return getPersonalStats(reqInfo);
	}
	RequestResult result;
	ErrorResponse response;
	response.message = "Imcompatibile request code";
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = nullptr;
	return result;
}

MenuRequestHandler::MenuRequestHandler(const LoggedUser& user)
{
	mUser = user;
	mFactory = RequestHandlerFactory::getInstance();
}

string MenuRequestHandler::getUsername()
{
	return mUser.getUsername();
}

RequestResult MenuRequestHandler::signout(const RequestInfo& reqInfo)
{
	mFactory->getLoginManager()->logout(mUser.getUsername());
	LogoutResponse response;
	response.status = SUCCESS;
	RequestResult result;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = mFactory->createLoginRequestHandler();
	return result;
}

RequestResult MenuRequestHandler::getRooms(const RequestInfo& reqInfo)
{
	GetRoomsResponse response;
	response.rooms = mFactory->getRoomManager()->getRooms();
	response.status = SUCCESS;
	RequestResult result;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = this;
	return result;
}

RequestResult MenuRequestHandler::getPlayersInRoom(const RequestInfo& reqInfo)
{
	GetPlayersInRoomRequest request = JsonRequestPacketDeserializer::deserializeGetPlayersRequest(reqInfo);
	GetPlayersInRoomResponse response;
	response.status = SUCCESS;
	response.players = mFactory->getRoomManager()->getRoom(request.roomId).getAllUsers();
	RequestResult result;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = this;
	return result;
}

RequestResult MenuRequestHandler::getPersonalStats(const RequestInfo& reqInfo)
{
	GetPersonalStatsResponse response;
	response.statistics = mFactory->getStatisticsManager()->getUserStatistics(mUser.getUsername());
	response.status = SUCCESS;
	RequestResult result;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = this;
	return result;
}

RequestResult MenuRequestHandler::getHighScore(const RequestInfo& reqInfo)
{
	GetHighScoreResponse response;
	response.statistics = mFactory->getStatisticsManager()->getHighScore();
	response.status = SUCCESS;
	RequestResult result;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = this;
	return result;
}

RequestResult MenuRequestHandler::joinRoom(const RequestInfo& reqInfo)
{
	JoinRoomRequest request = JsonRequestPacketDeserializer::deserializeJoinRoomRequest(reqInfo);
	Room* room = &(mFactory->getRoomManager()->getRoom(request.roomId));
	if (room->getAllUsers().size() >= room->getRoomData().maxPlayers)
	{
		ErrorResponse response;
		response.message = "The room is full!";
		RequestResult result;
		result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
		result.nextHandler = this;
		return result;
	}
	JoinRoomResponse response;
	bool res = room->addUser(mUser);
	response.status = res ? SUCCESS : FAILURE;
	RequestResult result;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = res ? mFactory->createRoomMemberRequestHandler(mUser, room) : (IRequestHandler*)this;
	return result;

}

RequestResult MenuRequestHandler::createRoom(const RequestInfo& reqInfo)
{
	CreateRoomRequest request = JsonRequestPacketDeserializer::deserializeCreateRoomRequest(reqInfo);
	RoomData roomData;
	roomData.maxPlayers = request.maxUsers;
	roomData.name = request.roomName;
	roomData.timePerQuestion = request.answerTimeout;
	roomData.numOfQuestionsInGame = request.questionCount;
	roomData.state = RoomState::OPENED;
	roomData.id = mFactory->getRoomManager()->getNextId();
	mFactory->getRoomManager()->createRoom(mUser, roomData);
	CreateRoomResponse response;
	response.status = SUCCESS;
	response.roomId = roomData.id;
	RequestResult result;
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = (IRequestHandler*)mFactory->createRoomAdminRequestHandler(mUser, &(mFactory->getRoomManager()->getRoom(roomData.id)));
	return result;
}
