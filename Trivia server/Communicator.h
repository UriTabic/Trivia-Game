#pragma once

#include <WinSock2.h>
#include <Windows.h>
#include <queue>
#include <string>
#include <mutex>
#include <thread>
#include <map>
#include "IRequestHandler.h"
#include "LoginRequestHandler.h"
using std::queue;
using std::mutex;
using std::string;
using std::thread;
using std::map;
#define FLAGS 0
#define HEADERS 5
#define CODE_INDEX 0
#define LEN_INDEX 1
#define NO_USER "\1"

/*
A class that is used for running a TCP server and handling client requests
*/
class Communicator
{
public:
	Communicator();
	~Communicator();
	/*
	* Starts to listen in the port that passed.
	*/
	void startHandleRequests(const int port);
private:
	/*
	* Accepts a client and opens a new thread for him.
	*/
	void bindAndListen();
	/*
	* Handles client's requests.
	*/
	void handleNewClient(SOCKET clientSocket);
	/*
	* Recieves len bytes into data.
	*/
	void recieve(SOCKET socket, char* data, const int len) const;
	/*
	* Sends len bytes from data.
	*/
	void sendPacket(SOCKET socket, const char* data, const int len) const;

	/*
	* Ensures that state machine logs out of all activity before closing the connection.
	*/
	void closeSafe(IRequestHandler* handler);

	SOCKET mServerSocket;
	RequestHandlerFactory* mHandlerFactory;
	map<SOCKET, IRequestHandler*> mClients;
	map<SOCKET, string> mUsernames;
};

