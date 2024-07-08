#include "Communicator.h"
#include <exception>
#include <iostream>
#include <string>
#include <thread>
#include <fstream>
#include <chrono>


Communicator::Communicator()
{
	mHandlerFactory = RequestHandlerFactory::getInstance();
	// this server use TCP. that why SOCK_STREAM & IPPROTO_TCP
	// if the server use UDP we will use: SOCK_DGRAM & IPPROTO_UDP
	mServerSocket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

	if (mServerSocket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__ " - socket");

	//Creates a thread that removes fnished games and rooms.
	GameManager::getInstance()->startRemoveFinishedGames().detach(); 

}

Communicator::~Communicator()
{
	try
	{
		// the only use of the destructor should be for freeing 
		// resources that was allocated in the constructor
		closesocket(mServerSocket);
	}
	catch (...) {}
	for (auto& it : mClients)
	{
		delete it.second;
		it.second = nullptr;
	}
	mClients.erase(mClients.begin(), mClients.end());
}

void Communicator::startHandleRequests(const int port)
{

	struct sockaddr_in sa = { 0 };

	sa.sin_port = htons(port); // port that server will listen for
	sa.sin_family = AF_INET;   // must be AF_INET
	sa.sin_addr.s_addr = INADDR_ANY;    // when there are few ip's for the machine. We will use always "INADDR_ANY"

	// Connects between the socket and the configuration (port and etc..)
	if (bind(mServerSocket, (struct sockaddr*)&sa, sizeof(sa)) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - bind");

	// Start listening for incoming requests of clients
	if (listen(mServerSocket, SOMAXCONN) == SOCKET_ERROR)
		throw std::exception(__FUNCTION__ " - listen");
	std::cout << "Listening on port " << port << std::endl;
	while (true)
	{
		// the main thread is only accepting clients 
		// and add then to the list of handlers
		std::cout << "Waiting for client connection request" << std::endl;
		bindAndListen();
	}
}


void Communicator::bindAndListen()
{

	// this accepts the client and create a specific socket from server to this client
	// the process will not continue until a client connects to the server
	SOCKET client_socket = accept(mServerSocket, NULL, NULL);
	if (client_socket == INVALID_SOCKET)
		throw std::exception(__FUNCTION__);

	std::cout << "Client accepted. Server and client can speak" << std::endl;
	// the function that handle the conversation with the client
	thread(&Communicator::handleNewClient, this, client_socket).detach();
}


void Communicator::handleNewClient(SOCKET clientSocket)
{
	//Initialize new user at beggining of state machine.
	mClients[clientSocket] = mHandlerFactory->createLoginRequestHandler();
	char data[HEADERS] = { 0 };
	//Currently user isn't signed in. 
	mUsernames[clientSocket] = NO_USER;
	bool loginTry = false;
	try
	{
		while(true)
		{
			recieve(clientSocket, data, HEADERS);
			//Extarct data from received message.
			unsigned char code = data[CODE_INDEX];
			loginTry = code == CODES::LOGIN_REQUEST;
			int* len = (int*)&(data[LEN_INDEX]);
			char* msg = new char[*len + 1];
			if(*len > 0) recieve(clientSocket, msg, *len);
			msg[*len] = 0;
			RequestInfo reqInfo(msg, code);
			std::cout << "receiving from " 
				<< ((mUsernames[clientSocket] == NO_USER) ? "socket " + std::to_string(clientSocket) : "user \"" + mUsernames[clientSocket] + "\"")
				<< std::endl;
			std::cout << reqInfo;
			delete[] msg;

			if (mClients[clientSocket]->isRequestRelevant(reqInfo))
			{
				try
				{
					RequestResult reqResult = mClients[clientSocket]->handleRequest(reqInfo);
					//Free memory if there is new state.
					if(reqResult.nextHandler != mClients[clientSocket])
					{
						delete(mClients[clientSocket]);
						mClients[clientSocket] = reqResult.nextHandler;
					}
					if (loginTry)
					{
						MenuRequestHandler* temp;
						if ((temp = dynamic_cast<MenuRequestHandler*>(mClients[clientSocket])) != nullptr)
						{
							mUsernames[clientSocket] = temp->getUsername();
						}
					}
					//Is logged out.
					if (dynamic_cast<LoginRequestHandler*>(mClients[clientSocket]) != nullptr)
					{
						mUsernames[clientSocket] = NO_USER;
					}
					sendPacket(clientSocket, reqResult.buffer.c_str(), reqResult.buffer.size());
				}
				catch (std::exception e)
				{
					throw std::runtime_error("Error sending data!");
				}
			}
			else
			{
 				throw std::runtime_error("Request is not relevant!");
			}
		}
		// Closing the socket (in the level of the TCP protocol)
		closesocket(clientSocket);
		mClients.erase(clientSocket);
		closeSafe(mClients[clientSocket]);
	}
	catch (const std::exception& e)
	{
		closesocket(clientSocket);
		closeSafe(mClients[clientSocket]);
		mClients.erase(clientSocket);
	}


}

void Communicator::recieve(SOCKET socket, char* data, const int len) const
{
	int res = recv(socket, data, len, FLAGS);
	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(socket);
		throw std::exception(s.c_str());
	}

}

void Communicator::sendPacket(SOCKET socket, const char* data, const int len) const
{
	std::cout << "\nSent " << std::to_string(len) << " bytes to " 
		<< ((mUsernames.at(socket) == NO_USER) ? "socket " + std::to_string(socket) : "user \"" + mUsernames.at(socket) + "\"")
		<< std::endl;

	std::cout << "CODE: " << get_code_string((CODES)data[0]) << std::endl;
	std::cout << "Data: " << string(data + HEADERS, len - HEADERS) << std::endl;
	int res = send(socket, data, len, FLAGS);
	if (res == INVALID_SOCKET)
	{
		std::string s = "Error while recieving from socket: ";
		s += std::to_string(socket);
		throw std::exception(s.c_str());
	}
}

void Communicator::closeSafe(IRequestHandler* handler)
{
	try 
	{
		if (dynamic_cast<LoginRequestHandler*>(handler) != nullptr) return;
		if (dynamic_cast<RoomAdminRequestHandler*>(handler) != nullptr)
		{
			RequestInfo info("", CODES::CLOSE_ROOM_REQUEST);
			RequestResult res = ((RoomAdminRequestHandler*)handler)->handleRequest(info);
			handler = res.nextHandler;
		}
		if (dynamic_cast<RoomMemberRequestHandler*>(handler) != nullptr)
		{
			RequestInfo info("", CODES::LEAVE_ROOM_REQUEST);
			RequestResult res = ((RoomMemberRequestHandler*)handler)->handleRequest(info);
			handler = res.nextHandler;
		}
		if (dynamic_cast<MenuRequestHandler*>(handler) != nullptr)
		{
			RequestInfo info("", CODES::LOGOUT_REQUEST);

			((MenuRequestHandler*)handler)->handleRequest(info);
		}

	}
	catch (std::exception e)
	{

	}
}
