/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* Header file that contains the declarations for the Win32 API functions used
* to create simple GUI window in the base station
*/

#ifndef BASE_UNIT_GUI_H
#define BASE_UNIT_GUI_H

#include "utilities.h"

#include <Windows.h>
#include <string>
#include <vector>
#include <iostream>
#include <string.h>
#include <conio.h>

using std::vector;
using std::ofstream;
using std::string;
using std::cout;
using std::endl;

#define ID_SELF_DESTROY_BUTTON 100
#define ID_LISTBOX 101
#define ID_CLOSE 102
#define ID_OKAY 103
#define ID_CONTINUE 104
#define ID_QUIT 105
#define ID_CONFIRM 106
#define ID_UNCONFIRMED 107


LRESULT CALLBACK WindowProcedure(HWND, UINT, WPARAM, LPARAM);
LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);

//void AddControls(HWND);
void AddText_NoSerial(HWND hWnd, HWND button);
void AddText_Serial(HWND hWnd);
void loadImages();

/*These two functions get the port with user input*/
BOOL getPort(vector<string>* v, string& name);
BOOL getPort1(vector<string>* listOfPorts, string& name);

/*These procedures use message boxes to get user input.*/
int closeProgram();
int possibleNetworkFailure(string name);
std::wstring stringToWString(const std::string& s);
BOOL closeProgram1();
LRESULT CALLBACK CloseHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam);
BOOL confirmNetworkFailure(string string);
LRESULT CALLBACK FailureHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp);


#endif // !BASE_UNIT_GUI_H