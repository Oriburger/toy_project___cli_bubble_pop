/// ----------------------------------
///	@file	socket.cpp
///	@data	2021/06/15
///	@brief
/// ----------------------------------
#include <iostream>
#include <mutex>
#include <thread>
#include <windows.h>
#include "socket.h"
#include "game.h"

/// 
///	@fn		bool SendData(SOCKET_COMPONENT& sc, const GameState* user, std::mutex& m)
/// @brief	������ ���� ������ ���� �÷��̾��� board ������ �����Ѵ�.
/// @return	true�� ��ȯ
/// 
bool SendData(SOCKET_COMPONENT& sc, GameState* user)
{
	const UserUIState& ui_state_ref = user->GetUIStateRef();

	while (user->IsPlaying())
	{
		char str[128] = "";

		for (int i = 0; i < ui_state_ref.board_len; i++)
		{
			for (int j = 0; j < ui_state_ref.board_len; j++)
				str[j] = ui_state_ref.board[i][j] + '0';
			str[ui_state_ref.board_len] = i + '0'; //�ش� board�� ��idx
			send(sc.client_socket, str, sizeof(str), 0);
			Sleep(200);
		}
	}
	return true;
}

/// 
///	@fn		bool SendData(SOCKET_COMPONENT& sc, const GameState* user, std::mutex& m)
/// @brief	������ ���� ������ ���� �÷��̾��� board ������ �����Ѵ�.
/// @return	true�� ��ȯ
/// 
bool RecvData(SOCKET_COMPONENT& sc, GameState* user)
{
	UserUIState& ui_state_ref = user->GetUIStateRef();

	while (user->IsPlaying())
	{
		char str[128] = "";

		for (int i = 0; i < ui_state_ref.board_len; i++)
		{
			int len = recv(sc.client_socket, str, sizeof(str), 0);
			if (len <= ui_state_ref.board_len || str[0]-'0'==-48)
			{
				i--;
				continue;
			}
			for (int j = 0; j < ui_state_ref.board_len; j++)
				ui_state_ref.board[str[ui_state_ref.board_len]-'0'][j] = str[j]-'0';
		}
	}
	return true;
}

/// 
///	@fn		void SyncData(SOCKET_COMPONENT& sc, GameState user[2])
/// @brief	���� �÷��̾��� �����͸� �۽��԰� ���ÿ�, ��� �÷��̾��� �����͸� ������.
/// @brief	���� �����带 ���� ����ó���� �����.
/// @see	SendData();
/// @see	RecvData();
/// @return	void
/// 
void SyncData(SOCKET_COMPONENT& sc, GameState user[2])
{
	std::thread t_send(SendData, std::ref(sc), &user[0]);
	std::thread t_recv(RecvData, std::ref(sc), &user[1]);

	t_send.join();
	t_recv.join();

	Sleep(1000);
}