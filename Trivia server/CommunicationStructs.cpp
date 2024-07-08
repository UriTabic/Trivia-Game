#include "CommunicationStructs.h"

#include <string>
#include <unordered_map>

std::string get_code_string(const CODES code) {
    static const std::unordered_map<CODES, std::string> code_map = {
        {CODES::LOGIN_REQUEST, "login request"},
        {CODES::LOGIN_RESPONSE, "login response"},
        {CODES::SIGNUP_REQUEST, "signup request"},
        {CODES::SIGNUP_RESPONSE, "signup response"},
        {CODES::ERROR_RESPONSE, "error response"},
        {CODES::LOGOUT_REQUEST, "logout request"},
        {CODES::LOGOUT_RESPONSE, "logout response"},
        {CODES::GET_PLAYERS_IN_ROOM_REQUEST, "get players in room request"},
        {CODES::GET_PLAYERS_IN_ROOM_RESPONSE, "get players in room response"},
        {CODES::JOIN_ROOM_REQUEST, "join room request"},
        {CODES::JOIN_ROOM_RESPONSE, "join room response"},
        {CODES::CREATE_ROOM_REQUEST, "create room request"},
        {CODES::CREATE_ROOM_RESPONSE, "create room response"},
        {CODES::GET_ROOMS_REQUEST, "get rooms request"},
        {CODES::GET_ROOMS_RESPONSE, "get rooms response"},
        {CODES::GET_HIGH_SCORE_REQUEST, "get high score request"},
        {CODES::GET_HIGH_SCORE_RESPONSE, "get high score response"},
        {CODES::GET_PERSONAL_STATS_REQUEST, "get personal stats request"},
        {CODES::GET_PERSONAL_STATS_RESPONSE, "get personal stats response"},
        {CODES::CLOSE_ROOM_REQUEST, "close room request"},
        {CODES::CLOSE_ROOM_RESPONSE, "close room response"},
        {CODES::START_GAME_REQUEST, "start game request"},
        {CODES::START_GAME_RESPONSE, "start game response"},
        {CODES::GET_ROOM_STATE_REQUEST, "get room state request"},
        {CODES::GET_ROOM_STATE_RESPONSE, "get room state response"},
        {CODES::LEAVE_ROOM_REQUEST, "leave room request"},
        {CODES::LEAVE_ROOM_RESPONSE, "leave room response"},
        {CODES::SUBMIT_ANSWER_REQUEST, "submit answer request"},
        {CODES::SUBMIT_ANSWER_RESPONSE, "submit answer response"},
        {CODES::GET_GAME_RESULT_REQUEST, "get game result request"},
        {CODES::GET_GAME_RESULT_RESPONSE, "get game result response"},
        {CODES::GET_QUESTION_REQUEST, "get question request"},
        {CODES::GET_QUESTION_RESPONSE, "get question response"},
        {CODES::LEAVE_GAME_REQUEST, "leave game request"},
        {CODES::LEAVE_GAME_RESPONSE, "leave game response"},
    };

    auto it = code_map.find(code);
    if (it != code_map.end()) {
        return it->second;
    }
    else {
        return "Unkown code";
    }
}