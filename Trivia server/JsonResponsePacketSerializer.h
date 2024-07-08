#pragma once

#include "CommunicationStructs.h"
#include "json.hpp"

/****
 * @brief The JsonResponsePacketSerializer class is responsible for serializing response structures into JSON format.
 *
 * This class contains static methods for converting response data structures, such as LoginResponse, SignupResponse, etc.,
 * into JSON-formatted strings suitable for network transmission.
 ****/
class JsonResponsePacketSerializer
{
public:
    /****
     * @brief Serializes a LoginResponse structure into a JSON string.
     *
     * This method converts the provided LoginResponse object into a JSON-formatted string.
     *
     * @param loginResponse A reference to a LoginResponse object.
     * @returns A JSON-formatted string representing the login response.
     ****/
    static std::string serializeResponse(const LoginResponse& loginResponse);

    /****
     * @brief Serializes a SignupResponse structure into a JSON string.
     *
     * This method converts the provided SignupResponse object into a JSON-formatted string.
     *
     * @param signupResponse A reference to a SignupResponse object.
     * @returns A JSON-formatted string representing the signup response.
     ****/
    static std::string serializeResponse(const SignupResponse& signupResponse);

    /****
     * @brief Serializes an ErrorResponse structure into a JSON string.
     *
     * This method converts the provided ErrorResponse object into a JSON-formatted string.
     *
     * @param errorResponse A reference to an ErrorResponse object.
     * @returns A JSON-formatted string representing the error response.
     ****/
    static std::string serializeResponse(const ErrorResponse& errorResponse);

    /****
     * @brief Serializes a LogoutResponse structure into a JSON string.
     *
     * This method converts the provided LogoutResponse object into a JSON-formatted string.
     *
     * @param logoutResponse A reference to a LogoutResponse object.
     * @returns A JSON-formatted string representing the logout response.
     ****/
    static std::string serializeResponse(const LogoutResponse& logoutResponse);

    /****
     * @brief Serializes a GetRoomsResponse structure into a JSON string.
     *
     * This method converts the provided GetRoomsResponse object into a JSON-formatted string.
     *
     * @param getRoomsResponse A reference to a GetRoomsResponse object.
     * @returns A JSON-formatted string representing the get rooms response.
     ****/
    static std::string serializeResponse(const GetRoomsResponse& getRoomsResponse);

    /****
     * @brief Serializes a GetPlayersInRoomResponse structure into a JSON string.
     *
     * This method converts the provided GetPlayersInRoomResponse object into a JSON-formatted string.
     *
     * @param getPlayersInRoomResponse A reference to a GetPlayersInRoomResponse object.
     * @returns A JSON-formatted string representing the get players in room response.
     ****/
    static std::string serializeResponse(const GetPlayersInRoomResponse& getPlayersInRoomResponse);

    /****
     * @brief Serializes a JoinRoomResponse structure into a JSON string.
     *
     * This method converts the provided JoinRoomResponse object into a JSON-formatted string.
     *
     * @param joinRoomResponse A reference to a JoinRoomResponse object.
     * @returns A JSON-formatted string representing the join room response.
     ****/
    static std::string serializeResponse(const JoinRoomResponse& joinRoomResponse);

    /****
     * @brief Serializes a CreateRoomResponse structure into a JSON string.
     *
     * This method converts the provided CreateRoomResponse object into a JSON-formatted string.
     *
     * @param createRoomResponse A reference to a CreateRoomResponse object.
     * @returns A JSON-formatted string representing the create room response.
     ****/
    static std::string serializeResponse(const CreateRoomResponse& createRoomResponse);

    /****
     * @brief Serializes a GetHighScoreResponse structure into a JSON string.
     *
     * This method converts the provided GetHighScoreResponse object into a JSON-formatted string.
     *
     * @param getHighScoreResponse A reference to a GetHighScoreResponse object.
     * @returns A JSON-formatted string representing the get high score response.
     ****/
    static std::string serializeResponse(const GetHighScoreResponse& getHighScoreResponse);

    /****
     * @brief Serializes a GetPersonalStatsResponse structure into a JSON string.
     *
     * This method converts the provided GetPersonalStatsResponse object into a JSON-formatted string.
     *
     * @param getPersonalStatsResponse A reference to a GetPersonalStatsResponse object.
     * @returns A JSON-formatted string representing the get personal stats response.
     ****/
    static std::string serializeResponse(const GetPersonalStatsResponse& getPersonalStatsResponse);

    /****
     * @brief Serializes a CloseRoomResponse structure into a JSON string.
     *
     * This method converts the provided CloseRoomResponse object into a JSON-formatted string.
     *
     * @param response A reference to a CloseRoomResponse object.
     * @returns A JSON-formatted string representing the close room response.
     ****/
    static std::string serializeResponse(const CloseRoomResponse& response);

    /****
     * @brief Serializes a StartGameResponse structure into a JSON string.
     *
     * This method converts the provided StartGameResponse object into a JSON-formatted string.
     *
     * @param response A reference to a StartGameResponse object.
     * @returns A JSON-formatted string representing the start game response.
     ****/
    static std::string serializeResponse(const StartGameResponse& response);

    /****
     * @brief Serializes a GetRoomStateResponse structure into a JSON string.
     *
     * This method converts the provided GetRoomStateResponse object into a JSON-formatted string.
     *
     * @param response A reference to a GetRoomStateResponse object.
     * @returns A JSON-formatted string representing the get room state response.
     ****/
    static std::string serializeResponse(const GetRoomStateResponse& response);

    /****
     * @brief Serializes a LeaveRoomResponse structure into a JSON string.
     *
     * This method converts the provided LeaveRoomResponse object into a JSON-formatted string.
     *
     * @param response A reference to a LeaveRoomResponse object.
     * @returns A JSON-formatted string representing the leave room response.
     ****/
    static std::string serializeResponse(LeaveRoomResponse& response);

    /****
     * @brief Serializes a GetGameResultsResponse structure into a JSON string.
     *
     * This method converts the provided GetGameResultsResponse object into a JSON-formatted string.
     *
     * @param response A reference to a GetGameResultsResponse object.
     * @returns A JSON-formatted string representing the get game results response.
     ****/
    static std::string serializeResponse(GetGameResultsResponse& response);

    /****
     * @brief Serializes a SubmitAnswerResponse structure into a JSON string.
     *
     * This method converts the provided SubmitAnswerResponse object into a JSON-formatted string.
     *
     * @param response A reference to a SubmitAnswerResponse object.
     * @returns A JSON-formatted string representing the submit answer response.
     ****/
    static std::string serializeResponse(SubmitAnswerResponse& response);

    /****
     * @brief Serializes a GetQuestionResponse structure into a JSON string.
     *
     * This method converts the provided GetQuestionResponse object into a JSON-formatted string.
     *
     * @param response A reference to a GetQuestionResponse object.
     * @returns A JSON-formatted string representing the get question response.
     ****/
    static std::string serializeResponse(GetQuestionResponse& response);

    /****
     * @brief Serializes a LeaveGameResponse structure into a JSON string.
     *
     * This method converts the provided LeaveGameResponse object into a JSON-formatted string.
     *
     * @param response A reference to a LeaveGameResponse object.
     * @returns A JSON-formatted string representing the leave game response.
     ****/
    static std::string serializeResponse(LeaveGameResponse& response);

private:
    /****
     * @brief Wraps a JSON message with protocol-specific information.
     *
     * This method adds protocol-specific headers, such as response code and message size,
     * to the JSON-formatted message string.
     *
     * @param code An integer representing the response code.
     * @param message A string containing the JSON-formatted message.
     * @returns A string representing the wrapped protocol message.
     ****/
    static std::string wrapToProtocol(const int code, const std::string message);
};
