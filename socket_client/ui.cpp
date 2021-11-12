/// ----------------------------------
///	@file	ui_val.cpp
///	@data	2021/06/16
///	@brief
/// ----------------------------------
#include <iostream>
#include <Windows.h>
#include <string>
#include <vector>
#include "ui.h"

//int game_ui[2][9][8]; ///< ���� ���࿡ ���Ǵ� UI ������ ����(�� ������ board)
const short BUBBLE_COLOR[5] = { YELLOW, RED, GREEN, BLUE, VIOLET }; ///<bubble�� ����
const std::string lineNumStr = "���������";
const std::vector < std::string> lineNum = { "��","��","��","��"
											,"��","��","��","��" };

///
/// @fn		void setColor(unsigned short text)
///	@brief	������ ����.
/// @return void
///
void setColor(unsigned short text)
{
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), text);
}

///
/// @fn		void gotoxy(int y, int x)
///	@brief	���ڷ� ���޵� ��ǥ�� Ŀ���� �̵�.
/// @return void
///
void gotoxy(int y, int x)
{
	COORD pos = { (SHORT)x, (SHORT)y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

///
/// @fn		void PrintUI(const UserUIState& ui);
///	@brief	game_ui �迭�� ���
/// @return void
///
void PrintUI(std::vector<UserUIState> ui_val)
{
	for (int id = 0; id <= 1; id++)
	{
		gotoxy(ui_val[id].ui_ypos - 1, ui_val[id].ui_xpos);
		std::cout << " " << lineNumStr;
		for (int i = 0; i < ui_val[id].board_len; i++)
		{
			gotoxy(ui_val[id].ui_ypos + i, ui_val[id].ui_xpos - 1);
			std::cout << lineNum[i];
			for (int j = 0; j < ui_val[id].board_len; j++)
			{
				if (ui_val[id].board[i][j])
				{
					setColor(BUBBLE_COLOR[ui_val[id].board[i][j] - 1]);
					std::cout << "��";
					setColor(WHITE);
				}
				else
					std::cout << "��";
			}
		}
	}
	//std::cout <<"score : "<< ;
}

///
/// @fn	void PrtMainMenu()
///	@todo ����
///
void PrtMainMenu()
{
	gotoxy(0, 0);
}