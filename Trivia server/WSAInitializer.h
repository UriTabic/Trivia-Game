#pragma once

#include <WinSock2.h>
#include <Windows.h>

/*
* A class used to run server safely.
*/
class WSAInitializer
{
public:
	WSAInitializer();
	~WSAInitializer();
};

