/// --------------------------------
///	@file	client_main.cpp
/// @data	2021/06/13
///	@brief
/// --------------------------------
#pragma comment(lib, "ws2_32.lib")
#include <iostream>
#include <ctime>
#include <string>
#include <cstring>
#include <cstdlib>
#include <windows.h>
#include <thread>
#include <utility>
#include <vector>
#include <mutex>
#include "game.h"
#include "socket.h"

int main()
{
	CONSOLE_CURSOR_INFO cursorInfo = { 0, };
	cursorInfo.dwSize = 1;
	cursorInfo.bVisible = FALSE;
	SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);

	//std::mutex m;
	SOCKET_COMPONENT sc;
	GameState state[2] = { GameState(true)
						 , GameState(false)};

	std::thread t1(InGame, state);
	std::thread t2(SyncData, std::ref(sc), state);

	t2.join();
	Sleep(3000);
	t1.join();

	sc.CleanSocket();
	return 0;
}