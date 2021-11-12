///	----------------------------------
///	@file	server_main.cpp
///	@data	2021/06/13
///	@brief
///	----------------------------------
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <string>
#include <thread>
#include <mutex>
#include <Windows.h>
#include "socket.h"

int main()
{
	//std::mutex m;

	SOCKET_COMPONENT sc;

	if(!ConnectClient(sc, USER_ID[0]) || !ConnectClient(sc, USER_ID[1]))
		std::cout << "Error : Can't connect to clients\n";

	std::thread com1(ExchangeData, std::ref(sc), USER_ID[0]);
	std::thread com2(ExchangeData, std::ref(sc), USER_ID[1]);

	com1.join();
	com2.join();

	sc.CleanSocket();
	return 0;
}