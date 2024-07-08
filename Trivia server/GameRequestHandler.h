#pragma once
#include "IRequestHandler.h"
#include "GameManager.h"
#include "RequestHandlerFactory.h"
#include "chrono"
class RequestHandlerFactory;

#define FALSE_ID 10
#define NANO_TO_DECI 1.0 / 100000000.0
#define TO_DECI 10

class GameRequestHandler : public IRequestHandler
{
public:
	GameRequestHandler(Game* game, const LoggedUser& user, const unsigned int answerTimeOut);
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

private:

	/**
	* @brief Handles a request to get a question for the current user.
	* @param info Request information containing request code and data.
	* @return Encapsulates the serialized response and request handler object.
	*/
	RequestResult getQuestion(RequestInfo info);

	/**
	* @brief Handles answer submission, tracks timeout.
	* @param info Request information containing request code and data.
	* @return Encapsulates the serialized response and request handler object.
	*/
	RequestResult submitAnswer(RequestInfo info);

	/**
	* @brief Provides game results and potentially transitions to menu.
	* @param info Request information containing request code and data.
	* @return Encapsulates the serialized response and request handler object.
	*/
	RequestResult getGameResults(RequestInfo info);

	/**
	* @brief Handles leaving the game, transitions to menu.
	* @param info Request information containing request code and data.
	* @return Encapsulates the serialized response and new menu handler.
	*
	* This method removes the player from the game using `mGame->removePlayer(mUser)`. It then builds a `LeaveGameResponse` and serializes it.
	* A new `MenuRequestHandler` is created using the factory (`mFacroty`) for the current user. The serialized response and the new menu handler are returned.
	*/
	RequestResult leaveGame(RequestInfo info);

	std::chrono::steady_clock::time_point mLastTime;//The time point the user asked for the question
	bool mAnswered; //Did he already answered the question and waits for the others
	Game* mGame; //Game instance
	LoggedUser mUser; //Current user
	GameManager* mGameManager; 
	RequestHandlerFactory* mFacroty;
	bool mLastRequest; //Does the questions over and need to go back to menu handler
	unsigned int mAnswerTimeout; //Maximum time per question

};

