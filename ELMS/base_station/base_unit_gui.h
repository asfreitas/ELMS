/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* Header file that contains the declarations for the Win32 API functions used
* to create simple GUI's in the base station
*/

#ifndef BASE_UNIT_GUI_H
#define BASE_UNIT_GUI_H
#include <Windows.h>
#include <string>
#include <vector>
#include "utilities.h"
#include <iostream>
#include <string.h>

using std::vector;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;


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

/*These two functions get the port with user input*/
BOOL getPort(vector<string>* v, string & name);
BOOL getPort1(vector<string>* listOfPorts, string & name );


#endif // !BASE_UNIT_GUI_H