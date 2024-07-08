#pragma once
#include "IRequestHandler.h"
#include "RequestHandlerFactory.h"
class RequestHandlerFactory;


class LoginRequestHandler : public IRequestHandler
{
public:
	LoginRequestHandler();
	virtual bool isRequestRelevant(const RequestInfo& reqInfo) override;
	virtual RequestResult handleRequest(const RequestInfo& reqInfo) override;

private:
	RequestHandlerFactory* mHandlerFactory;
	/**
	* Handles a login request.
	*
	* This private function deserializes the login request from the provided RequestInfo object,
	* calls the LoginManager's login() function to perform the login logic, and prepares a response.
	* On successful login, it creates a LoginResponse object and sets the next handler in the chain
	* to the MenuRequestHandler for the logged-in user. On failure, it creates an ErrorResponse object
	* with an appropriate error message and sets the next handler back to itself.
	*
	* @param reqInfo A reference to a RequestInfo object containing the login request information.
	* @return A RequestResult object containing the login response and potentially the next handler.
	*/
	RequestResult login(const RequestInfo& reqInfo);
	/**
	* Handles a signup request.
	*
	* This private function deserializes the signup request from the provided RequestInfo object,
	* calls the validRegistration() function to validate the signup data, and prepares a response.
	* If validation fails, it creates an ErrorResponse object with an appropriate error message
	* and sets the next handler back to itself. If validation succeeds, it calls the LoginManager's
	* signup() function to create the new user and prepares a SignupResponse object. On successful
	* signup, it creates a SignupResponse object and sets the next handler in the chain to the
	* MenuRequestHandler for the newly registered user. On failure (e.g., username already exists),
	* it creates an ErrorResponse object with an appropriate error message and sets the next handler
	* back to itself.
	*
	* @param reqInfo A reference to a RequestInfo object containing the signup request information.
	* @return A RequestResult object containing the signup response and potentially the next handler.
	*/
	RequestResult signup(const RequestInfo& reqInfo);
	/**
	* Validates user registration data.
	*
	* This private function performs various checks on the provided signup request data,
	* including username length, email format, and password strength. It returns a character code
	* indicating the type of validation error encountered (if any). 'v' indicates successful validation.
	*
	* @param request A reference to a SignupRequest object containing the user registration data.
	* @return A character code indicating the validation result.
	*/
	RESULTS validRegistaration(const SignupRequest& request) const;
};