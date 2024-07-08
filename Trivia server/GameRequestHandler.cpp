#include "GameRequestHandler.h"
#include "JsonResponsePacketSerializer.h"
#include "JsonRequestPacketDeserializer.h"
GameRequestHandler::GameRequestHandler(Game* game, const LoggedUser& user, const unsigned int answerTimeOut)
{
	mGame = game;
	mGame->addPlayer(user);
	mUser = user;
	mGameManager = GameManager::getInstance();
	mFacroty = RequestHandlerFactory::getInstance();
	mLastTime = std::chrono::steady_clock::now();
	mLastRequest = false;
	mAnswerTimeout = answerTimeOut;
	mAnswered = false;
}

bool GameRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case SUBMIT_ANSWER_REQUEST:
	case GET_QUESTION_REQUEST:
	case GET_GAME_RESULT_REQUEST:
	case LEAVE_GAME_REQUEST:
		return true;
	default:
		return false;
	};
}

RequestResult GameRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case SUBMIT_ANSWER_REQUEST:
		return submitAnswer(reqInfo);
	case GET_QUESTION_REQUEST:
		return getQuestion(reqInfo);
	case GET_GAME_RESULT_REQUEST:
		return getGameResults(reqInfo);
	case LEAVE_GAME_REQUEST:
		return leaveGame(reqInfo);
	}
	return RequestResult();
}

RequestResult GameRequestHandler::getQuestion(RequestInfo info)
{
	mAnswered = false;
	Question q("", vector<string>{""}, 0);
	try 
	{
		q = mGame->getQuestionForUser(mUser);
	}
	catch(std::exception e)
	{
		mLastRequest = true;
	}
	vector<string> answers = q.getPossibleAnswers();
	std::unordered_map<unsigned int, string> map;
	for (size_t i = 0; i < answers.size(); ++i) {
		map[i] = answers[i];
	}
	GetQuestionResponse response{mLastRequest ? FAILURE : SUCCESS, q.getQuestion(), map};
	RequestResult result{ JsonResponsePacketSerializer::serializeResponse(response), this };
	
	mLastTime = std::chrono::steady_clock::now();
	return result;
}

RequestResult GameRequestHandler::submitAnswer(RequestInfo info)
{
	static unsigned int id = FALSE_ID;
	unsigned int idToSend = FALSE_ID;
	auto delta = std::chrono::steady_clock::now() - mLastTime;
	int deciSeconds = delta.count() * NANO_TO_DECI;
	if (!mAnswered)
	{
		SubmitAnswerRequest request = JsonRequestPacketDeserializer::deserializeSubmitAnswerRequest(info);
		if (deciSeconds > mAnswerTimeout * TO_DECI)
		{
			deciSeconds = mAnswerTimeout * TO_DECI;
			request.answerId = FALSE_ID;
		}
		id = mGame->submitAnswer(request.answerId, mUser, deciSeconds);
		mAnswered = true;
	}
	int status = FAILURE;
	if (mGame->nextQuestion() || deciSeconds > (mAnswerTimeout + 2) * TO_DECI)
	{
		status = SUCCESS;
		idToSend = id;
	}
	else
	{
		status = FAILURE;
		idToSend = FALSE_ID;
	}
	SubmitAnswerResponse response{ status, id };
	return RequestResult{ JsonResponsePacketSerializer::serializeResponse(response), this };
}

RequestResult GameRequestHandler::getGameResults(RequestInfo info) 
{
	GetGameResultsResponse response{ SUCCESS, mGame->getResults() };
	
	return RequestResult{ JsonResponsePacketSerializer::serializeResponse(response), (mLastRequest ? mFacroty->createMenuRequestHandler(mUser) : (IRequestHandler*)this) };
}

RequestResult GameRequestHandler::leaveGame(RequestInfo info)
{
	mGame->removePlayer(mUser);
	LeaveGameResponse response{SUCCESS};
	return RequestResult {JsonResponsePacketSerializer::serializeResponse(response), mFacroty->createMenuRequestHandler(mUser)};
}
