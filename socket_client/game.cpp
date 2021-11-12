/// --------------------------------
///	@file	game.cpp
///	@data	2021/05/19
///	@brief	
/// --------------------------------
#include <iostream>
#include <string>
#include <vector>
#include <queue>
#include <utility>
#include <Windows.h>
#include <algorithm>
#include <thread>
#include <mutex>
#include <conio.h>
#include "game.h"
#include "socket.h"

#define MIN_POP_COUNT 4

///
/// @fn		void GameState::Debug()
///	@brief	GameState ��ü�� ���¸� ���.
///	@return	void.
///
void GameState::Debug()
{
	std::cout << "[Board]---\n";
	for (int i = 0; i < ui_val.board_len; i++)
	{
		for (int j = 0; j < ui_val.board_len; j++)
		{
			std::cout << ui_val.board[i][j] << ' ';
		}
		std::cout << '\n';
	}
	std::cout << "-------\n";
	std::cout << "[Can Pop?] : " << CanPopBoard() << '\n';
}

///
/// @fn		void GameState::InitBoard()
///	@brief	���������� ���带 �ʱ�ȭ.
///
void GameState::InitBoard()
{
	srand((unsigned)time(NULL));
	for (int i = 0; i < ui_val.board_len; i++)
		for (int j = 0; j < ui_val.board_len; j++)
			ui_val.board[i][j] = rand() % ui_val.bubble_var_cnt + 1;
}

///
/// @fn		bool GameState::CanPopBoard()
/// @see	BoardBFS();
///	@brief	���� ���尡 pop�� �� �� �ִ��� ���θ� boolean���·� ��ȯ.
///	@brief	��� ��ǥ���� BFS�� ���� �ּ� pop ������ Ž��.
///	@return	pop�� �����ϸ� true, �Ұ��ϸ� false.
///
bool GameState::CanPopBoard()
{
	int count = 1;

	//��� ��ǥ�� ���� BFS�Ѵ�.
	for (int p = 0; p < ui_val.board_len; p++)
		for (int q = 0; q < ui_val.board_len; q++)
			if (BoardBFS(*this, p, q).size() >= MIN_POP_COUNT)
				return true; //pop ����
	return false;
}

///
///	@fn		void InGame(GameState user[2])
/// @see	UpdateUI();
///	@see	PrintUI();
///	@see	GameState::CanPopBoard();
///	@see	GameState::InitBoard();
///	@brief	������ �����ϰ� ����.
///
void InGame(GameState user[2])
{
	//�÷��̰� ����Ǵ� ���� �ݺ�

	while (user[0].IsPlaying())
	{
		const std::vector<UserUIState> temp = { user[0].ui_val, user[1].ui_val };
		std::thread uiThread(PrintUI, temp); //I/O �и�
		uiThread.join();

		gotoxy(12, 0);

		if (!user[0].CanPopBoard())
		{
			user[0].InitBoard();
			continue;
		}

		std::string cmd = "";
		std::cout << " Input : ";
		if(_kbhit()) 
			std::cin >> cmd;

		if (cmd == "exit") break;

		if (cmd.size() == 2)
		{
			int y = cmd[0] - '0' - 1;
			int x = cmd[1] - '0' - 1;

			if (y < 0 || y >= user[0].ui_val.board_len
				|| x < 0 || x >= user[0].ui_val.board_len) continue;
			else
			{
				int pop_count = PopBoard(&user[0], y, x);
				user[0].SetScore(user[0].GetScore() + pop_count);
			}
		}
	}
}

///
/// @fn		std::vector<std::pair<int, int> > BoardBFS(const GameState& g1, const int y, const int x)
///	@brief	Breath-First-Search �˰��� ������� ���带 Ž��.
///	@return	(y, x)���� pop�� �����ϴٸ�, pop�Ǵ� bubble���� ��ǥ �迭�� ��ȯ.
///
std::vector<std::pair<int, int> > BoardBFS(const GameState& g1, const int y, const int x)
{
	const int dy[4] = { 1, -1, 0, 0 };
	const int dx[4] = { 0, 0, 1, -1 };

	int count = 1;
	std::pair<int, int> start_pos;
	std::vector<std::vector<bool> > visited;
	
	start_pos = { y, x };
	std::queue<std::pair<int, int> > pos_queue;
	visited = std::vector<std::vector<bool> >
		(g1.ui_val.board_len, std::vector<bool>(g1.ui_val.board_len, false));

	visited[y][x] = true;
	pos_queue.push(start_pos);
	while (!pos_queue.empty())
	{
		std::pair<int, int> curr_pos = pos_queue.front();
		int curr_bubble_type = g1.ui_val.board[curr_pos.first][curr_pos.second];
		pos_queue.pop();

		for (int dir_idx = 0; dir_idx < 4; dir_idx++)
		{
			const int next_ypos = curr_pos.first + dy[dir_idx];
			const int next_xpos = curr_pos.second + dx[dir_idx];
			const int next_bubble_type = g1.ui_val.board[next_ypos][next_xpos];

			//board�� ������ ����ٸ�
			if (next_ypos < 0 || next_xpos < 0
				|| next_ypos >= g1.ui_val.board_len || next_xpos >= g1.ui_val.board_len) continue;
			//next ĭ�� bubble ������ �ٸ��ٸ�
			if (curr_bubble_type != next_bubble_type)	continue;
			//�̹� �湮�� ���ٸ�
			if (visited[next_ypos][next_xpos]) continue;

			visited[next_ypos][next_xpos] = true;
			pos_queue.push({ next_ypos, next_xpos });
			count += 1;
		}
	}
	
	//pop ������ �ּ� ������ ���� ���Ѵٸ�, �� �迭�� ��ȯ.
	if (count < MIN_POP_COUNT) return {};

	std::vector < std::pair<int, int> > ret;
	for (int i = 0; i < g1.ui_val.board_len; i++)
		for (int j = 0; j < g1.ui_val.board_len; j++)
			if (visited[i][j]) ret.push_back({ i, j });

	return ret;
}

///
/// @fn		int PopBoard(GameState * g1, const int y, const int x)
/// @see	BoardBFS();
///	@brief	y, x ��ǥ�� pop �Ѵ�.
///	@return	pop�� bubble�� ������ ��ȯ.
///
int PopBoard(GameState* g1, const int y, const int x)
{
	std::vector<std::pair<int, int> > pop_bubble_list;

	pop_bubble_list = BoardBFS(*g1, y, x);

	if (!pop_bubble_list.size()) return 0;
	
	for (auto& pos : pop_bubble_list)
		g1->ui_val.board[pos.first][pos.second] = 0;
	
	for (int i = 0; i < g1->ui_val.board_len; i++)
	{
		std::queue<int> q;
		for (int j = g1->ui_val.board_len-1; j >= 0; j--)
		{
			if(g1->ui_val.board[j][i])
				q.push(g1->ui_val.board[j][i]);
			g1->ui_val.board[j][i] = 0;
		}

		for (int j = g1->ui_val.board_len - 1; j >= 0; j--)
		{
			if (q.empty()) break;

			g1->ui_val.board[j][i] = q.front();
			q.pop();
		}
	}
	for (int i = 0; i < g1->ui_val.board_len; i++)
		for (int j = 0; j < g1->ui_val.board_len; j++)
			if(g1->ui_val.board[i][j]==0)
				g1->ui_val.board[i][j] = rand() % 5 + 1;

	return (int)pop_bubble_list.size();
}