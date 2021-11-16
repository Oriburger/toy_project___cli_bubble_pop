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
	bool is_playing; ///< ���� ���� ����
	bool is_main_user; ///< �÷��̾� Ÿ�� ����, ��� �÷��̾� : false
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
	void Debug(void); ///< GameState ��ü�� ���¸� ���
	
	//---Get/Set Value---
	bool IsPlaying() const { return is_playing; }
	bool IsMainUser() const { return is_main_user; }
	int GetScore() const { return score; }
	void SetScore(int val) { score = val; }

	//---UI---
	UserUIState& GetUIState() { return ui_val; }
	UserUIState& GetUIState() const { return ui_val; }

	//---Game Function---
	void InitBoard(void); ///< ���� ������ ���� Board�� �ʱ�ȭ

	bool CanPopBoard(void);	///< Board�� pop������ ĭ�� �ִ��� ã��
};

///������ �����ϰ�, ����
void InGame(GameState user[2]);
///BFS ������� ���带 Ž���Ͽ� �ش� ��ǥ���� pop �Ǵ� ��ǥ ������ ��ȯ
std::vector<std::pair<int, int> > BoardBFS(const GameState& g1, const int y, const int x);
///�ش� ��ǥ�� pop�ϰ�, pop�� bubble�� ������ ��ȯ
int PopBoard(GameState &g1, const int y, const int x);