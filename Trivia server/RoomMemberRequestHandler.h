#pragma once

#include "RequestHandlerFactory.h"
#include "IRequestHandler.h"

/****
 * @brief Handles requests related to room members.
 *
 * The RoomMemberRequestHandler class is responsible for processing requests
 * from room members, such as leaving the room and getting the room state.
 ****/
class RoomMemberRequestHandler : public IRequestHandler
{
public:
    /****
     * @brief Constructs a RoomMemberRequestHandler.
     *
     * @param room Pointer to the room being managed.
     * @param user The logged-in user who is a room member.
     ****/
    RoomMemberRequestHandler(Room* room, const LoggedUser& user);

    /**
     * @brief Determines if a request is relevant to the current handler.
     *
     * This virtual function allows a handler to specify the types of requests
     * it can handle. It should return true if the provided RequestInfo object
     * matches the criteria for this handler, and false otherwise.
     *
     * @param reqInfo A reference to a RequestInfo object containing information about the request.
     * @return True if the request is relevant to this handler, false otherwise.
     */
    virtual bool isRequestRelevant(const RequestInfo& reqInfo) override;

    /**
     * @brief Handles an incoming request.
     *
     * This virtual function defines the core logic for processing a request.
     * It should take a RequestInfo object as input and return a RequestResult object.
     * The RequestResult object can contain a buffer with any response data and a pointer
     * to the next handler in the chain of responsibility (if applicable).
     *
     * @param reqInfo A reference to a RequestInfo object containing information about the request.
     * @return A RequestResult object containing the response data and potentially the next handler.
     */
    virtual RequestResult handleRequest(const RequestInfo& reqInfo) override;

private:
    /****
     * @brief Handles the get room state request.
     *
     * @param request The request information.
     * @returns A RequestResult object containing the response data.
     ****/
    RequestResult getRoomState(const RequestInfo request);

    /****
     * @brief Handles the leave room request.
     *
     * @param request The request information.
     * @returns A RequestResult object containing the response data.
     ****/
    RequestResult leaveRoom(const RequestInfo request);

    Room* mRoom;
    LoggedUser mUser;
    RoomManager* mRoomManager;
    RequestHandlerFactory* mFactory;
};
