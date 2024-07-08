#pragma comment (lib, "ws2_32.lib")

#include "WSAInitializer.h"
#include "Server.h"
#include <iostream>
#include <exception>
#include "CommunicationStructs.h"
#include "WSAInitializer.h"


int main()
{
	try
	{
		WSAInitializer wsaInit;
		Server* server = Server::getInstance();
		server->run();	
	}
	catch (std::exception& e)
	{
		std::cout << "Error occured: " << e.what() << std::endl;
	}

	return 0;
}