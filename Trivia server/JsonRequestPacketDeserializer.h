#pragma once

#include "CommunicationStructs.h"

/****
 * @brief The JsonRequestPacketDeserializer class is responsible for deserializing JSON data into request structures.
 *
 * This class contains static methods for converting JSON-formatted strings received in RequestInfo objects
 * into corresponding request data structures, such as LoginRequest, SignupRequest, etc.
 ****/
class JsonRequestPacketDeserializer
{
public:
    /****
     * @brief Deserializes a login request from a JSON buffer.
     *
     * This method parses the provided JSON data buffer and extracts the username and password
     * to populate a LoginRequest structure.
     *
     * @param buffer A reference to a RequestInfo object containing the JSON data.
     * @returns A LoginRequest structure containing the deserialized data.
     ****/
    static LoginRequest deserializeLoginRequest(const RequestInfo& buffer);

    /****
     * @brief Deserializes a signup request from a JSON buffer.
     *
     * This method parses the provided JSON data buffer and extracts the username, password,
     * and email to populate a SignupRequest structure.
     *
     * @param buffer A reference to a RequestInfo object containing the JSON data.
     * @returns A SignupRequest structure containing the deserialized data.
     ****/
    static SignupRequest deserializeSignupRequest(const RequestInfo& buffer);

    /****
     * @brief Deserializes a get players in room request from a JSON buffer.
     *
     * This method parses the provided JSON data buffer and extracts the room ID
     * to populate a GetPlayersInRoomRequest structure.
     *
     * @param buffer A reference to a RequestInfo object containing the JSON data.
     * @returns A GetPlayersInRoomRequest structure containing the deserialized data.
     ****/
    static GetPlayersInRoomRequest deserializeGetPlayersRequest(const RequestInfo& buffer);

    /****
     * @brief Deserializes a join room request from a JSON buffer.
     *
     * This method parses the provided JSON data buffer and extracts the room ID
     * to populate a JoinRoomRequest structure.
     *
     * @param buffer A reference to a RequestInfo object containing the JSON data.
     * @returns A JoinRoomRequest structure containing the deserialized data.
     ****/
    static JoinRoomRequest deserializeJoinRoomRequest(const RequestInfo& buffer);

    /****
     * @brief Deserializes a create room request from a JSON buffer.
     *
     * This method parses the provided JSON data buffer and extracts the room name, maximum users,
     * answer timeout, and question count to populate a CreateRoomRequest structure.
     *
     * @param buffer A reference to a RequestInfo object containing the JSON data.
     * @returns A CreateRoomRequest structure containing the deserialized data.
     ****/
    static CreateRoomRequest deserializeCreateRoomRequest(const RequestInfo& buffer);

    /****
     * @brief Deserializes a submit answer request from a JSON buffer.
     *
     * This method parses the provided JSON data buffer and extracts the answer ID
     * to populate a SubmitAnswerRequest structure.
     *
     * @param buffer A reference to a RequestInfo object containing the JSON data.
     * @returns A SubmitAnswerRequest structure containing the deserialized data.
     ****/
    static SubmitAnswerRequest deserializeSubmitAnswerRequest(const RequestInfo& buffer);
};
