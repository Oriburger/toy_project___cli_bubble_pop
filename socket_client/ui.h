/// ----------------------------------
///	@file	ui.h
///	@data	2021/06/16
///	@brief
/// ----------------------------------
#pragma once

///
/// @brief  bubble�� ������ ��Ÿ���� ������ bubble_type
///
enum bubble_type
{
    BLACK,DARK_BLUE, DARK_GREEN,
    DARK_SKYBLUE, DARK_RED, DARK_VOILET,
    DAKR_YELLOW, GRAY, DARK_GRAY, BLUE,
    GREEN, SKYBLUE, RED, VIOLET, YELLOW, WHITE
};

struct UserUIState
{
    static const int board_len = 8; ///< ������ �Ѻ� ����
    static const int bubble_var_cnt = 5; ///< bubble�� ���� ����

    int ui_ypos, ui_xpos; ///<ui ���� ��µ� ��ǥ�� ����
    int board[board_len][board_len]; //���� ���¸� �����ϴ� board �迭 
};

void setColor(unsigned short text);
void gotoxy(int y, int x);
///ui�迭�� ���
void PrintUI(std::vector<UserUIState> ui_val);