/// ----------------------------------
///	@file	socket.h
///	@data	2021/06/13
///	@brief
/// ----------------------------------
#pragma once
#include "game.h"

///
///	@struct	SOCKET_COMPONENT
/// @brief	소켓 통신을 위한 멤버들을 명시해놓은 구조체.
/// 
struct SOCKET_COMPONENT
{
	WSADATA wsaData; ///< 윈속 초기화를 위한 변수
	SOCKET client_socket; ///< client의 소켓을 저장
	const int server_port; ///< 서버의 포트를 저장
	SOCKADDR_IN server_address; ///< 서버의 주소를 저장

	SOCKET_COMPONENT() : server_port(9876)
	{
		///Winsock2를 초기화
		if (WSAStartup(MAKEWORD(2, 2), &wsaData)) err_quit("Init Winsock2");

		///소켓 통신의 과정 - socket
		client_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (client_socket == INVALID_SOCKET) err_quit("socket()");

		///소켓 통신의 과정 - connect
		memset(&server_address, 0, sizeof(server_address));
		server_address.sin_family = AF_INET;
		server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
		server_address.sin_port = htons(server_port);

		std::cout << "connect to server ... ";
		for (int i = 0; i < 5; i++)
		{
			if (connect(client_socket, (SOCKADDR*)&server_address
				, sizeof(server_address)) != SOCKET_ERROR) break;

			std::cout << "  Failed, retry\n";
			Sleep(2000);
		}
		std::cout << "Complete!\n";
	}
	
	///에러가 생길 시, 에러문을 출력하고 종료
	void err_quit(const char* msg)
	{
		std::cout << msg << '\n';	
		exit(1);
	}

	///윈속과 소켓을 닫는다.
	void CleanSocket(void)
	{
		closesocket(client_socket);
		WSACleanup();
	}
};

/// 소켓을 통해 서버로 현재 플레이어의 board 정보를 전송한다.
bool SendData(SOCKET_COMPONENT& sc, GameState* user);

/// 소켓을 통해 서버로 상태 플레이어의 board 정보를 수신한다.
bool RecvData(SOCKET_COMPONENT& sc, GameState* user);

/// 현재 플레이어의 데이터를 송신함과 동시에, 상대 플레이어의 데이터를 수신함.
void SyncData(SOCKET_COMPONENT& sc, GameState user[2]);