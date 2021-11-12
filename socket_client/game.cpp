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
///	@brief	GameState 객체의 상태를 출력.
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
///	@brief	난수값으로 보드를 초기화.
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
///	@brief	현재 보드가 pop을 할 수 있는지 여부를 boolean형태로 반환.
///	@brief	모든 좌표에서 BFS를 돌며 최소 pop 개수를 탐색.
///	@return	pop이 가능하면 true, 불가하면 false.
///
bool GameState::CanPopBoard()
{
	int count = 1;

	//모든 좌표를 돌며 BFS한다.
	for (int p = 0; p < ui_val.board_len; p++)
		for (int q = 0; q < ui_val.board_len; q++)
			if (BoardBFS(*this, p, q).size() >= MIN_POP_COUNT)
				return true; //pop 가능
	return false;
}

///
///	@fn		void InGame(GameState user[2])
/// @see	UpdateUI();
///	@see	PrintUI();
///	@see	GameState::CanPopBoard();
///	@see	GameState::InitBoard();
///	@brief	게임을 시작하고 진행.
///
void InGame(GameState user[2])
{
	//플레이가 진행되는 동안 반복

	while (user[0].IsPlaying())
	{
		const std::vector<UserUIState> temp = { user[0].ui_val, user[1].ui_val };
		std::thread uiThread(PrintUI, temp); //I/O 분리
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
///	@brief	Breath-First-Search 알고리즘 기반으로 보드를 탐색.
///	@return	(y, x)에서 pop이 가능하다면, pop되는 bubble들의 좌표 배열을 반환.
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

			//board의 범위를 벗어난다면
			if (next_ypos < 0 || next_xpos < 0
				|| next_ypos >= g1.ui_val.board_len || next_xpos >= g1.ui_val.board_len) continue;
			//next 칸의 bubble 색깔이 다르다면
			if (curr_bubble_type != next_bubble_type)	continue;
			//이미 방문을 헀다면
			if (visited[next_ypos][next_xpos]) continue;

			visited[next_ypos][next_xpos] = true;
			pos_queue.push({ next_ypos, next_xpos });
			count += 1;
		}
	}
	
	//pop 가능한 최소 개수를 만족 못한다면, 빈 배열을 반환.
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
///	@brief	y, x 좌표를 pop 한다.
///	@return	pop한 bubble의 개수를 반환.
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