#pragma once
#include "IRequestHandler.h"
#include "LoggedUser.h"
#include "RequestHandlerFactory.h"
class RequestHandlerFactory;

/**
 * @brief Handles requests related to user menus (logout, rooms, stats, etc.).
 * @ingroup Handlers
 *
 * This class implements the `IRequestHandler` interface and is responsible for processing requests related to user menus.
 * It can handle actions like logging out, retrieving room information, getting user statistics, and more.
 */
class MenuRequestHandler : public IRequestHandler
{
public:
	/**
	* Determines if a request is relevant to the current handler.
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
	* Handles an incoming request.
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

	/**
   * @brief Constructor for the MenuRequestHandler.
   *
   * This constructor initializes a new `MenuRequestHandler` instance with the provided `LoggedUser` object.
   * The `LoggedUser` information is likely used to identify the user and potentially personalize menu responses.
   *
   * @param user A reference to a `LoggedUser` object representing the user associated with the request.
   */
	MenuRequestHandler(const LoggedUser& user);

	/**
   * @brief Retrieves the username of the associated user.
   *
   * This function provides access to the username of the user associated with this handler instance.
   *
   * @return The username of the logged-in user.
   */
	string getUsername();
private:
	LoggedUser mUser;
	RequestHandlerFactory* mFactory;


	/**
	* @brief Handles logout request, calls factory logout and builds LogoutResponse.
	* @param reqInfo A reference to a `RequestInfo` object containing information about the request.
	* @return RequestResult with serialized LogoutResponse and LoginRequestHandler as next handler.
	*/
	RequestResult signout(const RequestInfo& reqInfo);

	/**
	 * @brief Retrieves room list from factory, builds GetRoomsResponse.
	 * @param reqInfo A reference to a `RequestInfo` object containing information about the request.
	 * @return RequestResult with serialized GetRoomsResponse and this as next handler.
	 */
	RequestResult getRooms(const RequestInfo& reqInfo);

	/**
	 * @brief Parses request to get room ID, retrieves players from factory, builds GetPlayersInRoomResponse.
	 * @param reqInfo A reference to a `RequestInfo` object containing information about the request.
	 * @return RequestResult with serialized GetPlayersInRoomResponse and this as next handler.
	 */
	RequestResult getPlayersInRoom(const RequestInfo& reqInfo);

	/**
	 * @brief Retrieves user statistics from factory, builds GetPersonalStatsResponse.
	 * @param reqInfo A reference to a `RequestInfo` object containing information about the request.
	 * @return RequestResult with serialized GetPersonalStatsResponse.
	 */
	RequestResult getPersonalStats(const RequestInfo& reqInfo);

	/**
	 * @brief Retrieves high score from factory, builds GetHighScoreResponse.
	 * @param reqInfo A reference to a `RequestInfo` object containing information about the request.
	 * @return RequestResult with serialized GetHighScoreResponse.
	 */
	RequestResult getHighScore(const RequestInfo& reqInfo);

	/**
	 * @brief Attempts to join room based on request, builds JoinRoomResponse.
	 * @param reqInfo A reference to a `RequestInfo` object containing information about the request.
	 * @return RequestResult with serialized JoinRoomResponse and potentially RoomMemberRequestHandler as next handler (on success).
	 */
	RequestResult joinRoom(const RequestInfo& reqInfo);

	/**
	 * @brief Creates room using request data and factory, builds CreateRoomResponse.
	 * @param reqInfo A reference to a `RequestInfo` object containing information about the request.
	 * @return RequestResult with serialized CreateRoomResponse and potentially RoomAdminRequestHandler as next handler.
	 */
	RequestResult createRoom(const RequestInfo& reqInfo);

};

