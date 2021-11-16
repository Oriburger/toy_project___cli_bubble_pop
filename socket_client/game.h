/// --------------------------------
///	@file	game.h
///	@data	2021/05/19
///	@brief	
/// --------------------------------
#pragma once
#include <vector>
#include <mutex>
#include "ui.h"

class GameState
{
private:
	bool is_playing; ///< 게임 진행 여부
	bool is_main_user; ///< 플레이어 타입 지정, 상대 플레이어 : false
	int score = 0;
	mutable UserUIState ui_val;

public:
	GameState() : is_main_user(false), is_playing(true)
	{
		ui_val.ui_ypos = 2;
		ui_val.ui_xpos = is_main_user ? 1 : 13;
		do
		{
			InitBoard();
		} while (!CanPopBoard());
	}

	GameState(bool user_type)
		: is_main_user(user_type), is_playing(true)
	{
		ui_val.ui_ypos = 2;
		ui_val.ui_xpos = is_main_user ? 1 : 30;

		do
		{
			InitBoard();
		} while (!CanPopBoard());
	}

	//---Debug---
	void Debug(void); ///< GameState 객체의 상태를 출력
	
	//---Get/Set Value---
	bool IsPlaying() const { return is_playing; }
	bool IsMainUser() const { return is_main_user; }
	int GetScore() const { return score; }
	void SetScore(int val) { score = val; }

	//---UI---
	UserUIState& GetUIState() { return ui_val; }
	UserUIState& GetUIState() const { return ui_val; }

	//---Game Function---
	void InitBoard(void); ///< 게임 진행을 위해 Board를 초기화

	bool CanPopBoard(void);	///< Board에 pop가능한 칸이 있는지 찾음
};

///게임을 시작하고, 진행
void InGame(GameState user[2]);
///BFS 기반으로 보드를 탐색하여 해당 좌표에서 pop 되는 좌표 집합을 반환
std::vector<std::pair<int, int> > BoardBFS(const GameState& g1, const int y, const int x);
///해당 좌표를 pop하고, pop한 bubble의 개수를 반환
int PopBoard(GameState &g1, const int y, const int x);