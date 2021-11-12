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

const int board_len = 8; ///< 수신받을 board의 한 변 길이
char board[2][board_len][board_len]; ///< 양쪽 클라이언트로부터 수신받을 정보를 저장할 배열

///	
///	@fn		bool ConnectClient(SOCKET_COMPONENT& sc, int id)
///	@brief	listen 상태에 있는 클라이언트와 연결한다.
///	@return	클라이언트와 연결에 성공할 경우 true, 그렇지 않다면 false를 반환.
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
///	@brief	식별자가 id인 클라이언트로 상대방의 게임 정보를 송신
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
///	@brief	식별자가 id인 클라이언트로의 게임 정보를 수신
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
///	@brief	클라이언트와의 송,수신을 동시에 진행하여 데이터를 갱신
///	@brief	다중 쓰레드를 통한 병렬처리가 적용됨.
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