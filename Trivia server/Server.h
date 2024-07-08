#pragma once

#include "Communicator.h"
#include "SqliteDataBase.h"
#define PORT 8175

/*
* A singelton class that describes a server.
*/
class Server
{
public:
	/*
	* Runs the server socket and waits for user input for stop.
	*/
	void run();
	Server(const Server& obj) = delete; //no copy ctor in singelton class
	static Server* getInstance();
private:
	Server();
	~Server();
	static Server* instancePtr;
	Communicator mCommunicator;
	IDataBase* mDb;
};

