#include "LoginRequestHandler.h"
#include "JsonRequestPacketDeserializer.h"
#include "JsonResponsePacketSerializer.h"
#include <regex>

LoginRequestHandler::LoginRequestHandler()
{
	mHandlerFactory = RequestHandlerFactory::getInstance();
}

bool LoginRequestHandler::isRequestRelevant(const RequestInfo& reqInfo)
{
	return reqInfo.code == CODES::LOGIN_REQUEST || reqInfo.code == CODES::SIGNUP_REQUEST;
}

RequestResult LoginRequestHandler::handleRequest(const RequestInfo& reqInfo)
{
	switch (reqInfo.code)
	{
	case CODES::LOGIN_REQUEST:
		return login(reqInfo);
	case CODES::SIGNUP_REQUEST:
		return signup(reqInfo);
	default:
		break;
	}
	RequestResult result;
	ErrorResponse response;
	response.message = "Imcompatibile request code";
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = nullptr;
	return result;
}

RequestResult LoginRequestHandler::login(const RequestInfo& reqInfo)
{
	LoginRequest request = JsonRequestPacketDeserializer::deserializeLoginRequest(reqInfo);
	RequestResult reqResult;
	char res;
	if ((res = mHandlerFactory->getLoginManager()->login(request.username, request.password)) == RESULTS::VALID)
	{
		LoginResponse loginResponse;
		loginResponse.status = SUCCESS;
		reqResult.buffer = JsonResponsePacketSerializer::serializeResponse(loginResponse);
		reqResult.nextHandler = mHandlerFactory->createMenuRequestHandler(LoggedUser(request.username));
		return reqResult;
	}
	RequestResult result;
	ErrorResponse response;
	if(res == RESULTS::MISMATCH)	response.message = "Username not found or password and username doesn't match.";
	if(res == RESULTS::LOGGED)	response.message = "User already logged in.";
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = this;
	return result;
}

RequestResult LoginRequestHandler::signup(const RequestInfo& reqInfo)
{
	SignupRequest request = JsonRequestPacketDeserializer::deserializeSignupRequest(reqInfo);
	RequestResult reqResult;
	RESULTS res;
	if ((res = validRegistaration(request)) != RESULTS::VALID)
	{
		ErrorResponse response;
		if (res == RESULTS::WEAK_PASSWORD)
		{
			response.message = "Password isn't strong enough!";
		}
		else if (res == RESULTS::ILLEGAL_EMAIL)
		{
			response.message = "Invalid email address.";
		}
		else if (res == RESULTS::SHORT_USERNAME)
		{
			response.message = "Username is too short.";
		}
		reqResult.buffer = JsonResponsePacketSerializer::serializeResponse(response);
		reqResult.nextHandler = this;
		return reqResult;
	}
	if (mHandlerFactory->getLoginManager()->signup(request.username, request.password, request.email))
	{
		SignupResponse signupResponse;
		signupResponse.status = SUCCESS;
		reqResult.buffer = JsonResponsePacketSerializer::serializeResponse(signupResponse);
		reqResult.nextHandler = mHandlerFactory->createMenuRequestHandler(LoggedUser(request.username));
		return reqResult;
	}
	RequestResult result;
	ErrorResponse response;
	response.message = "Username already exists.";
	result.buffer = JsonResponsePacketSerializer::serializeResponse(response);
	result.nextHandler = this;
	return result;
}

RESULTS LoginRequestHandler::validRegistaration(const SignupRequest& request) const
{
	if(request.username.size() < 5)
	{
		return RESULTS::SHORT_USERNAME;
	}
	const std::regex pattern("([a-zA-Z0-9\\.]+)@([a-zA-Z\\.]+)\\.([a-zA-Z]{2,4})");

	// Check if the email matches the pattern
	if (!std::regex_match(request.email, pattern)) 
	{
		return RESULTS::ILLEGAL_EMAIL;
	}
	int minLength = 8; // Minimum password length
	int hasLower = 0;
	int hasUpper = 0;
	int hasDigit = 0;
	int hasSpecial = 0;

	// Check length
	if (request.password.length() < minLength) {
		return RESULTS::WEAK_PASSWORD;
	}

	// Iterate through characters and check for requirements
	for (char c : request.password) {
		if (islower(c)) {
			hasLower = 1;
		}
		else if (isupper(c)) {
			hasUpper = 1;
		}
		else if (isdigit(c)) {
			hasDigit = 1;
		}
		else if (ispunct(c)) {
			hasSpecial = 1;
		}
	}

	// Check if all requirements are met
	if (!(hasLower && hasUpper && hasDigit && hasSpecial))
	{
		return RESULTS::WEAK_PASSWORD;
	}


	return RESULTS::VALID;
}
