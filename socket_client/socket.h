/// ----------------------------------
///	@file	socket.h
///	@data	2021/06/13
///	@brief
/// ----------------------------------
#pragma once
#include "game.h"

///
///	@struct	SOCKET_COMPONENT
/// @brief	���� ����� ���� ������� ����س��� ����ü.
/// 
struct SOCKET_COMPONENT
{
	WSADATA wsaData; ///< ���� �ʱ�ȭ�� ���� ����
	SOCKET client_socket; ///< client�� ������ ����
	const int server_port; ///< ������ ��Ʈ�� ����
	SOCKADDR_IN server_address; ///< ������ �ּҸ� ����

	SOCKET_COMPONENT() : server_port(9876)
	{
		///Winsock2�� �ʱ�ȭ
		if (WSAStartup(MAKEWORD(2, 2), &wsaData)) err_quit("Init Winsock2");

		///���� ����� ���� - socket
		client_socket = socket(AF_INET, SOCK_STREAM, 0);
		if (client_socket == INVALID_SOCKET) err_quit("socket()");

		///���� ����� ���� - connect
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
	
	///������ ���� ��, �������� ����ϰ� ����
	void err_quit(const char* msg)
	{
		std::cout << msg << '\n';	
		exit(1);
	}

	///���Ӱ� ������ �ݴ´�.
	void CleanSocket(void)
	{
		closesocket(client_socket);
		WSACleanup();
	}
};

/// ������ ���� ������ ���� �÷��̾��� board ������ �����Ѵ�.
bool SendData(SOCKET_COMPONENT& sc, GameState* user);

/// ������ ���� ������ ���� �÷��̾��� board ������ �����Ѵ�.
bool RecvData(SOCKET_COMPONENT& sc, GameState* user);

/// ���� �÷��̾��� �����͸� �۽��԰� ���ÿ�, ��� �÷��̾��� �����͸� ������.
void SyncData(SOCKET_COMPONENT& sc, GameState user[2]);