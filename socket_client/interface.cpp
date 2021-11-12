#include "interface.h"
#include <iostream>
#include <windows.h>

void gotoxy(int y, int x)
{
	COORD pos = { x,y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}