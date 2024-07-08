#include "JsonResponsePacketSerializer.h"
#include <bitset>

using json = nlohmann::json;

std::string JsonResponsePacketSerializer::serializeResponse(const LoginResponse& loginResponse)
{
	json jsonMsg;
	jsonMsg["status"] = loginResponse.status;
	return wrapToProtocol(CODES::LOGIN_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::serializeResponse(const SignupResponse& signupResponse)
{
	json jsonMsg;
	jsonMsg["status"] = signupResponse.status;
	return wrapToProtocol(CODES::SIGNUP_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::serializeResponse(const ErrorResponse& errorResponse)
{
	return wrapToProtocol(CODES::ERROR_RESPONSE, errorResponse.message);
}

std::string JsonResponsePacketSerializer::serializeResponse(const LogoutResponse& logoutResponse)
{
	json jsonMsg;
	jsonMsg["status"] = logoutResponse.status;
	return wrapToProtocol(CODES::LOGOUT_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::serializeResponse(const GetRoomsResponse& getRoomsResponse)
{
	json jsonMsg;
	json rooms = json::array(); //create dict.
	for (auto it = getRoomsResponse.rooms.begin(); it != getRoomsResponse.rooms.end(); ++it)
	{
		json room;
		room["id"] = it->id;
		room["state"] = it->state;
		room["maxPlayers"] = it->maxPlayers;
		room["name"] = it->name;
		room["numOfQuestionsInGame"] = it->numOfQuestionsInGame;
		room["timePerQuestion"] = it->timePerQuestion;
		rooms.push_back(room);
	}

	jsonMsg["Rooms"] = rooms;
	jsonMsg["status"] = getRoomsResponse.status;
	return wrapToProtocol(CODES::GET_ROOMS_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResponse)
{
	string players = "";
	json jsonMsg;
	jsonMsg["status"] = getPlayersInRoomResponse.status;
	jsonMsg["players"] = getPlayersInRoomResponse.players;
	return wrapToProtocol(CODES::GET_PLAYERS_IN_ROOM_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::serializeResponse(const JoinRoomResponse& joinRoomResponse)
{
	json jsonMsg;
	jsonMsg["status"] = joinRoomResponse.status;
	return wrapToProtocol(CODES::JOIN_ROOM_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::serializeResponse(const CreateRoomResponse& createRoomResponse)
{
	json jsonMsg;
	jsonMsg["status"] = createRoomResponse.status;
	jsonMsg["roomId"] = createRoomResponse.roomId;
	return wrapToProtocol(CODES::CREATE_ROOM_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::serializeResponse(const GetHighScoreResponse& getHighScoreResponse)
{
	json jsonMsg;
	jsonMsg["status"] = getHighScoreResponse.status;
	jsonMsg["statistics"] = getHighScoreResponse.statistics;
	return wrapToProtocol(CODES::GET_HIGH_SCORE_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::serializeResponse(const GetPersonalStatsResponse& getPersonalStatsResponse)
{
	json jsonMsg;
	jsonMsg["status"] = getPersonalStatsResponse.status;
	jsonMsg["statistics"] = getPersonalStatsResponse.statistics;
	return wrapToProtocol(CODES::GET_PERSONAL_STATS_RESPONSE, jsonMsg.dump());
}

string JsonResponsePacketSerializer::serializeResponse(const CloseRoomResponse& response)
{
	json jsonMsg;
	jsonMsg["status"] = response.status;
	return wrapToProtocol(CODES::CLOSE_ROOM_RESPONSE, jsonMsg.dump());
}

string JsonResponsePacketSerializer::serializeResponse(const StartGameResponse& response)
{
	json jsonMsg;
	jsonMsg["status"] = response.status;
	return wrapToProtocol(CODES::START_GAME_RESPONSE, jsonMsg.dump());
}

string JsonResponsePacketSerializer::serializeResponse(const GetRoomStateResponse& response)
{
	json jsonMsg;
	jsonMsg["status"] = response.status;
	jsonMsg["questionCount"] = response.questionCount;
	jsonMsg["answerTimeout"] = response.answerTimeout;
	jsonMsg["state"] = response.state;
	jsonMsg["players"] = response.players;
	return wrapToProtocol(CODES::GET_ROOM_STATE_RESPONSE, jsonMsg.dump());
}

string JsonResponsePacketSerializer::serializeResponse(LeaveRoomResponse& response)
{
	json jsonMsg;
	jsonMsg["status"] = response.status;
	return wrapToProtocol(CODES::LEAVE_ROOM_RESPONSE, jsonMsg.dump());
}

string JsonResponsePacketSerializer::serializeResponse(GetGameResultsResponse& response)
{
	json jsonMsg;
	jsonMsg["status"] = response.status;
	json results = json::array();
	for (auto it = response.results.begin(); it != response.results.end(); ++it)
	{
		json room;
		room["username"] = it->username;
		room["correctAnswerCount"] = it->correctAnswerCount;
		room["wrongAnswerCount"] = it->wrongAnswerCount;
		room["averageAnswerTime"] = it->averageAnswerTime;
		room["hasRetired"] = it->hasRetired;
		results.push_back(room);
	}
	jsonMsg["results"] = results;
	return wrapToProtocol(CODES::GET_GAME_RESULT_RESPONSE, jsonMsg.dump());
}

string JsonResponsePacketSerializer::serializeResponse(SubmitAnswerResponse& response)
{
	json jsonMsg;
	jsonMsg["status"] = response.status;
	jsonMsg["correctAnswerId"] = response.correctAnswerId;
	return wrapToProtocol(CODES::SUBMIT_ANSWER_RESPONSE, jsonMsg.dump());
}

string JsonResponsePacketSerializer::serializeResponse(GetQuestionResponse& response)
{
	json jsonMsg;
	jsonMsg["status"] = response.status;
	jsonMsg["question"] = response.question;

	json answers = json::object();
	for (auto& it : response.answers)
	{
		answers[std::to_string((int)it.first)] = it.second;
	}

	jsonMsg["answers"] = answers;
	return wrapToProtocol(CODES::GET_QUESTION_RESPONSE, jsonMsg.dump());
}

string JsonResponsePacketSerializer::serializeResponse(LeaveGameResponse& response)
{
	json jsonMsg;
	jsonMsg["status"] = response.status;
	return wrapToProtocol(CODES::LEAVE_GAME_RESPONSE, jsonMsg.dump());
}

std::string JsonResponsePacketSerializer::wrapToProtocol(const int code, const std::string message)
{
	std::string response = "";
	response += code;
	unsigned int size = message.size();
	char* c = (char*)&size;
	response += std::string(c, sizeof(int));
	response += message;

	return response;
}
