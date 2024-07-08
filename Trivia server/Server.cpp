#include "Server.h"
#include <iostream>
#include <fstream>
using std::cout;
Server* Server::instancePtr = nullptr;

Server::Server()
{
	mDb = SqliteDataBase::getInstance();
}

void Server::run()
{
	int port = PORT;
	std::ifstream configFile("config.txt");
	if (configFile.good()) {
		string content = "";
		std::getline(configFile, content);
		try {
			port = atoi(content.c_str() + 7);
			if (port == 0 || content.rfind("port = ") != 0) port = PORT;
		}catch (std::exception& e) {}
	}
	//Init servewr on different thread.
	thread(&Communicator::startHandleRequests, &mCommunicator, port).detach();

	string input = "";
	//Run until user enters exit;
	while (input != "EXIT")
	{
		std::getline(std::cin, input);
	}

	system("PAUSE");

}

Server* Server::getInstance()
{
	if (instancePtr == nullptr)
	{
		instancePtr = new Server();
	}
	return instancePtr;
}

Server::~Server()
{
	delete instancePtr;
}
