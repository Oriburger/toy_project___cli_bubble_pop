/// ----------------------------------
///	@file	ui.h
///	@data	2021/06/16
///	@brief
/// ----------------------------------
#pragma once

///
/// @brief  bubble의 색상을 나타내는 열겨형 bubble_type
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
    static const int board_len = 8; ///< 보드의 한변 길이
    static const int bubble_var_cnt = 5; ///< bubble의 종류 개수

    int ui_ypos, ui_xpos; ///<ui 에서 출력될 좌표를 저장
    int board[board_len][board_len]; //게임 상태를 저장하는 board 배열 
};

void setColor(unsigned short text);
void gotoxy(int y, int x);
///ui배열을 출력
void PrintUI(std::vector<UserUIState> ui_val);