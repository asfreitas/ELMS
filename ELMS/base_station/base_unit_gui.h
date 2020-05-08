/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* Header file that contains the declarations for the Win32 API functions used
* to create simple GUI's in the base station
*/


#ifndef BASE_UNIT_GUI_H
#define BASE_UNIT_GUI_H
#include <windows.h>
#include <string>
#include <iostream>
using std::string;

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define CHANGE_TITLE 4


LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);

void startWindow();
void AddMenus(HWND hWnd);
void AddControls(HWND);


#endif // !BASE_UNIT_GUI_H