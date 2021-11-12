///	----------------------------------
///	@file	socket.cpp
///	@data	2021/06/15
///	@brief
///	----------------------------------
#include <iostream>
#include <thread>
#include <mutex>
#include <Windows.h>
#include "socket.h"

const int board_len = 8; ///< ���Ź��� board�� �� �� ����
char board[2][board_len][board_len]; ///< ���� Ŭ���̾�Ʈ�κ��� ���Ź��� ������ ������ �迭

///	
///	@fn		bool ConnectClient(SOCKET_COMPONENT& sc, int id)
///	@brief	listen ���¿� �ִ� Ŭ���̾�Ʈ�� �����Ѵ�.
///	@return	Ŭ���̾�Ʈ�� ���ῡ ������ ��� true, �׷��� �ʴٸ� false�� ��ȯ.
///	
bool ConnectClient(SOCKET_COMPONENT& sc, int id)
{
	std::cout << "Connect to client #" << id << "...\n";

	//accept()-----------
	int size_client_addr = sizeof(sc.client_address[id]);
	sc.client_socket[id] = accept(sc.server_socket
					, (SOCKADDR*)&(sc.client_address[id]), &size_client_addr);
	if (sc.client_socket[id] == INVALID_SOCKET) return false;
	std::cout << "Accepted!\n";

	return true;
}

///	
///	@fn		bool SendData(SOCKET_COMPONENT& sc, int id)
///	@brief	�ĺ��ڰ� id�� Ŭ���̾�Ʈ�� ������ ���� ������ �۽�
///	@return	true
///	
bool SendData(SOCKET_COMPONENT& sc, int id)
{
	while (1)
	{
		char str[128] = "";

		for (int i = 0; i < board_len; i++)
		{
			for (int j = 0; j < board_len; j++)
				str[j] = board[!id][i][j];
			str[board_len] = i+'0';
			send(sc.client_socket[!id], str, sizeof(str), 0);
			Sleep(100);
		}
	}
	return true;
}

///	
///	@fn		bool RecvData(SOCKET_COMPONENT& sc, int id)
///	@brief	�ĺ��ڰ� id�� Ŭ���̾�Ʈ���� ���� ������ ����
///	@return	true
///	
bool RecvData(SOCKET_COMPONENT& sc, int id)
{
	while (1)
	{
		char str[128] = "";

		for (int i = 0; i < board_len; i++)
		{
			int len = recv(sc.client_socket[!id], str, sizeof(str), 0);
			if (len <= board_len || str[0] - '0' == -48)
			{
				i--;
				continue;
			}
			for (int j = 0; j < board_len; j++)
				board[id][str[board_len]-'0'][j] = str[j];
		}
	}
	return true;
}

///	
///	@fn		void ExchangeData(SOCKET_COMPONENT& sc, int id)
///	@brief	Ŭ���̾�Ʈ���� ��,������ ���ÿ� �����Ͽ� �����͸� ����
///	@brief	���� �����带 ���� ����ó���� �����.
///	@see	SendData();
///	@see	RecvData();
///	@return	void
///	
void ExchangeData(SOCKET_COMPONENT& sc, int id)
{
	std::thread t_send(SendData, std::ref(sc), id);
	std::thread t_recv(RecvData, std::ref(sc), id);

	t_recv.join();
	t_send.join();

	Sleep(1000);
}