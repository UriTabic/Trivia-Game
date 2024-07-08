#pragma once

#define _CRT_SECURE_NO_WARNINGS

#include "Room.h"
#include <string>
#include <ctime>
#include <iostream>
#include <unordered_map>

using std::string;
using std::vector;

#define SUCCESS 1
#define FAILURE 0

/*
* codes for tcp communication.
*/
enum CODES {
	LOGIN_REQUEST = 0, LOGIN_RESPONSE,
	SIGNUP_REQUEST, SIGNUP_RESPONSE,
	ERROR_RESPONSE,
	LOGOUT_REQUEST, LOGOUT_RESPONSE,
	GET_PLAYERS_IN_ROOM_REQUEST, GET_PLAYERS_IN_ROOM_RESPONSE,
	JOIN_ROOM_REQUEST, JOIN_ROOM_RESPONSE,
	CREATE_ROOM_REQUEST, CREATE_ROOM_RESPONSE,
	GET_ROOMS_REQUEST, GET_ROOMS_RESPONSE,
	GET_HIGH_SCORE_REQUEST, GET_HIGH_SCORE_RESPONSE,
	GET_PERSONAL_STATS_REQUEST, GET_PERSONAL_STATS_RESPONSE,
	CLOSE_ROOM_REQUEST, CLOSE_ROOM_RESPONSE,
	START_GAME_REQUEST, START_GAME_RESPONSE,
	GET_ROOM_STATE_REQUEST, GET_ROOM_STATE_RESPONSE,
	LEAVE_ROOM_REQUEST, LEAVE_ROOM_RESPONSE,
	SUBMIT_ANSWER_REQUEST, SUBMIT_ANSWER_RESPONSE,
	GET_QUESTION_REQUEST, GET_QUESTION_RESPONSE,
	GET_GAME_RESULT_REQUEST, GET_GAME_RESULT_RESPONSE,
	LEAVE_GAME_REQUEST, LEAVE_GAME_RESPONSE
};

/*
* Translate the code to its meaning.
* @param code - the code to translate
* @returns the code name.
*/
string get_code_string(const CODES code);

/*
* A struct that represents a login request.
*/
struct LoginRequest
{
	string username;
	string password;
};

/*
* A struct that represents a signup request.
*/
struct SignupRequest
{
	string username;
	string password;
	string email;
};

/*
* A struct that represents information about a received request.
*/
struct RequestInfo
{
	unsigned char code;
	std::time_t receivalTime;
	string data;

	RequestInfo(const char* msg, unsigned char status_code)
	{
		code = status_code;
		data = string(msg);
		std::time(&receivalTime);
	}
	friend std::ostream& operator<<(std::ostream& os, const RequestInfo reqInfo)
	{
		os << "Code: " << get_code_string((CODES)reqInfo.code) << std::endl;
		char timeString[26];
		ctime_s(timeString, sizeof(timeString), &(reqInfo.receivalTime));
		os << "Receival time: " << timeString;
		os << "Data: " << reqInfo.data << std::endl;
		return os;
	}
};

/*
* A struct that represents a login response.
*/
struct LoginResponse
{
	unsigned int status;
};

/*
* A struct that represents a signup response.
*/
struct SignupResponse
{
	unsigned int status;
};

/*
* A struct that represents a logout response.
*/
struct LogoutResponse
{
	unsigned int status;
};

/*
* A struct that represents a response containing a list of rooms.
*/
struct GetRoomsResponse
{
	unsigned int status;
	vector<RoomData> rooms;
};

/*
* A struct that represents a request to get players in a room.
*/
struct GetPlayersInRoomRequest
{
	unsigned int roomId;
};

/*
* A struct that represents a response containing a list of players in a room.
*/
struct GetPlayersInRoomResponse
{
	unsigned int status;
	vector<string> players;
};

/*
* A struct that represents a response containing high score information.
*/
struct GetHighScoreResponse
{
	unsigned int status;
	vector<string> statistics;
};

/*
* A struct that represents a response containing personal statistics information.
*/
struct GetPersonalStatsResponse
{
	unsigned int status;
	vector<string> statistics;
};

/*
* A struct that represents a request to join a room.
*/
struct JoinRoomRequest
{
	unsigned int roomId;
};

/*
* A struct that represents a response for joining a room.
*/
struct JoinRoomResponse
{
	unsigned int status;
};

/*
* A struct that represents a request to create a room.
*/
struct CreateRoomRequest
{
	string roomName;
	unsigned int maxUsers;
	unsigned int questionCount;
	unsigned int answerTimeout;
};

/*
* A struct that represents a response for creating a room.
*/
struct CreateRoomResponse
{
	unsigned int status;
	unsigned int roomId;
};

/*
* A struct that represents a response for closing a room.
*/
struct CloseRoomResponse
{
	unsigned int status;
};

/*
* A struct that represents a response for starting a game.
*/
struct StartGameResponse 
{
	unsigned int status;
};

/*
* A struct that represents a response for leaving a room.
*/
struct LeaveRoomResponse
{
	unsigned int status;
};

/*
* A struct that represents a response containing the state of a room.
*/
struct GetRoomStateResponse
{
	unsigned int status,
		questionCount,
		answerTimeout,
		state;
	vector<string> players;
};

struct LeaveGameResponse
{
	unsigned int status;
};

struct GetQuestionResponse 
{
	unsigned int status;
	string question;
	std::unordered_map<unsigned int, string> answers;
};

struct SubmitAnswerRequest
{
	unsigned int answerId;
};

struct SubmitAnswerResponse
{
	unsigned int status;
	unsigned int correctAnswerId;
};

struct PlayerResults 
{

	string username;
	unsigned int correctAnswerCount,
		wrongAnswerCount,
		averageAnswerTime,
		hasRetired;
};

struct GetGameResultsResponse
{
	unsigned int status;
	vector<PlayerResults> results;
};

/*
* A struct that represents an error response.
*/
struct ErrorResponse
{
	string message;
};
