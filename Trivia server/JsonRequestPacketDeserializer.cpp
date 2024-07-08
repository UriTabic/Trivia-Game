#include "JsonRequestPacketDeserializer.h"
#include "json.hpp"

using json = nlohmann::json;

LoginRequest JsonRequestPacketDeserializer::deserializeLoginRequest(const RequestInfo& buffer)
{
	LoginRequest lr;
	json data = json::parse(buffer.data);
	lr.password = data["password"];
	lr.username = data["username"];
	return lr;
}

SignupRequest JsonRequestPacketDeserializer::deserializeSignupRequest(const RequestInfo& buffer)
{
	SignupRequest sr;
	json data = json::parse(buffer.data);
	sr.password = data["password"];
	sr.username = data["username"];
	sr.email = data["email"];
	return sr;
}

GetPlayersInRoomRequest JsonRequestPacketDeserializer::deserializeGetPlayersRequest(const RequestInfo& buffer)
{
	GetPlayersInRoomRequest request;
	json data = json::parse(buffer.data);
	request.roomId = data["roomId"];
	return request;
}

JoinRoomRequest JsonRequestPacketDeserializer::deserializeJoinRoomRequest(const RequestInfo& buffer)
{
	JoinRoomRequest request;
	json data = json::parse(buffer.data);
	request.roomId = data["roomId"];
	return request;
}

CreateRoomRequest JsonRequestPacketDeserializer::deserializeCreateRoomRequest(const RequestInfo& buffer)
{
	CreateRoomRequest request;
	json data = json::parse(buffer.data);
	request.roomName = data["roomName"];
	request.maxUsers = data["maxUsers"];
	request.answerTimeout = data["answerTimeout"];
	request.questionCount = data["questionCount"];
	return request;
}

SubmitAnswerRequest JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(const RequestInfo& buffer)
{
	SubmitAnswerRequest request;
	json data = json::parse(buffer.data);
	request.answerId = data["answerId"];
	return request;
}
