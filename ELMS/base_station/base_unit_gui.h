/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* Header file that contains the declarations for the Win32 API functions used
* to create simple GUI's in the base station
*/


#ifndef BASE_UNIT_GUI_H
#define BASE_UNIT_GUI_H
#include <windows.h>
#include "port.h"
#include <string>
#include <iostream>
#include <cstring>
#include <vector>
using std::vector;
using std::string;

#define FILE_MENU_NEW 1
#define FILE_MENU_OPEN 2
#define FILE_MENU_EXIT 3
#define CHANGE_TITLE 4
#define ID_SELF_DESTROY_BUTTON 100
#define ID_LISTBOX 101


LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

void startWindow();
void AddMenus(HWND hWnd);
void AddControls(HWND);
bool getPort(vector<string>*, LPCSTR&);

#endif // !BASE_UNIT_GUI_H