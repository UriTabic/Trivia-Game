#pragma once
#include "CommunicationStructs.h"
#include <vector>
using std::vector;

class IRequestHandler;
/**
 * Struct to store request processing results.
 *
 * This struct encapsulates the response data (buffer) and a pointer to the next
 * IRequestHandler in the chain of responsibility (if applicable).
 */
struct RequestResult {
	string buffer;           // String buffer containing the response data.
	IRequestHandler* nextHandler;  // Pointer to the next handler in the chain (optional).
};

class IRequestHandler 
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
	virtual bool isRequestRelevant(const RequestInfo& reqInfo) = 0;
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
	virtual RequestResult handleRequest(const RequestInfo& reqInfo) = 0;
};