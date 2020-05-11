/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* This file contans definitions for functions that use the Win32 API to create
* GUI Windows
*/
#include "base_unit_gui.h"

//https://www.youtube.com/watch?v=8GCvZs55mEM
//You tube reference called:
// Windows GUI Programming with C/C++ ( Win32 API ) | Part -1 | Creating a window

// declare needed globals
HWND hEdit;
HWND hList;
char comPort[50];

/*
===================================================================
LRESULT CALLBACK WindowProcedure

This function calls back to the send message sent by the window. 
===================================================================
*/
LRESULT CALLBACK WindowProcedure(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg)
	{
	case WM_COMMAND:
		switch (wp)
		{

		case CHANGE_TITLE:
			wchar_t text[100];
			GetWindowTextW(hEdit, text, 100);
			SetWindowTextW(hWnd, text);
			break;

		case FILE_MENU_EXIT:
			DestroyWindow(hWnd);
			break;
		case FILE_MENU_NEW:
			MessageBeep(MB_ICONINFORMATION);
			break;
		case FILE_MENU_OPEN:
			MessageBeep(MB_ICONWARNING);
			break;
		}
		break;

	case WM_CREATE:
		AddMenus(hWnd);
		AddControls(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);

		break;

	default:
		return DefWindowProcW(hWnd, msg, wp, lp);
	}
}
/*                           
====================================================
startWindow
This function declares and opens a very rudimentary
window.
===================================================
*/
void startWindow()
{
	WNDCLASSW wc = { 0 };
	wc.hbrBackground = (HBRUSH)COLOR_WINDOW;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = GetModuleHandle(NULL);
	wc.lpszClassName = L"myWindowClass";
	wc.lpfnWndProc = WindowProcedure;

	if (RegisterClassW(&wc) == 0)
		printf("The class failed to register\n");

	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZEBOX,
		100, 100, 500, 500, NULL, NULL, NULL, NULL);
	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

/*
===================================================================
AddMenus
This function is part of a tutorial to create a basic window
===================================================================
*/
void AddMenus(HWND handle)
{
	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenuA(hSubMenu, MF_STRING, CHANGE_TITLE, "Change Title");

	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR)hSubMenu, "Open Submenu");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");

	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, "File");
	AppendMenu(hMenu, MF_STRING, NULL, "Help");
	SetMenu(handle, hMenu);

}

/*
===========================================================================
AddControls
This function is part of a tutorial to add a text box that allows the user
to enter text. 
==============================================================================
*/
void AddControls(HWND hWnd)
{
	CreateWindowW(L"static", L"Enter Text In The Box Below ", WS_VISIBLE | WS_CHILD |
		WS_BORDER | SS_CENTER, 200, 100, 100, 50, hWnd, NULL, NULL, NULL);
	hEdit = CreateWindowW(L"edit", L" ", WS_VISIBLE | WS_CHILD | WS_BORDER |
		ES_MULTILINE | ES_AUTOVSCROLL, 200, 152, 100, 50, hWnd, NULL, NULL, NULL);

}

/*
============================================================================
getPort
This is a "wrapper function" for getPort1 which creates the Window with the 
list box and returns the users selected port.  In order to get that returned
text and copy it into a string that was sent, this wrapper function is used. 
The SendMessage functions for the Window does not allow extra parameters, so this
function captures the selected port that was put into the global string variable
comPort in getPort1
==============================================================================
*/
BOOL getPort(vector<string>* v, string & name )
{
	getPort1(v, name);
	name = comPort;

	return 0;
}

/*                     
==============================================================================
LRESULT CALLBACK MessageHandler
This function is the call back for the listbox that was created. 
https://stackoverflow.com/questions/42438135/c-winapi-listbox-getting-selected-item-using-lb-getsel-lb-getcursel
/https://gist.github.com/Pilzschaf/d950a86042c37a9c8d1a8b9b5f082fff
=============================================================================
*/
LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:

		if (LOWORD(wParam) == ID_SELF_DESTROY_BUTTON) {
			//if the button is pushed
			char buffer[50];
			//get the list number that was selected
			DWORD64 dwSel = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (dwSel != LB_ERR)
			{
				//retrieve the text
				SendMessage(hList, LB_GETTEXT, dwSel, (LPARAM)(LPCSTR) buffer);
				//copy it into the global string
				strcpy_s(comPort, buffer);
				break;
			}
		}
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}

/*
=========================================================================
getPort1
This function creates the listbox Window.
//https://gist.github.com/Pilzschaf/d950a86042c37a9c8d1a8b9b5f082fff
*/
BOOL getPort1(vector<string>* listOfPorts, string & name)
{
	HINSTANCE hInstance = GetModuleHandle(0);
	HWND hWnd;
	HWND hButton;
	WNDCLASS wc;
	MSG msg;

	wc = {};
	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MessageHandler;
	wc.hInstance = hInstance;
	wc.hbrBackground = (HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = "SELECT_PORT";

	if (!RegisterClass(&wc))
		std::cout << "Failed to register" << std::endl;

	hWnd = CreateWindowW(L"SELECT_PORT", L"Select a COM port", WS_OVERLAPPEDWINDOW |
		WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	hButton = CreateWindowW(L"button", L"Select Port and Click This Button", WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 70, 300, 50, hWnd, (HMENU)ID_SELF_DESTROY_BUTTON,
		hInstance, 0);
	hList = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE |
		WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL | LBS_NOTIFY, 400, 40, 150, 200, 
		hWnd, (HMENU)ID_LISTBOX, 0, 0);
	//if the listOfPorts is empty, then tell the user and exit
	if (listOfPorts->empty())
	{
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"No Ports Detected");
	}
	//otherwise, convert the list of ports to messages that will be added to
	// the list in the listbox.
	else
	{
		for (size_t i = 0; i < listOfPorts->size(); i++)
		{
			string str = listOfPorts->at(i);
			// convert str to c-string
			const char* temp1 = str.c_str();
			LPCTSTR portname = (LPCTSTR)temp1;
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)portname);
		}
		//https://stackoverflow.com/questions/42438135/c-winapi-listbox-getting-selected-item-using-lb-getsel-lb-getcursel	
	}

	while (true) {
		BOOL result = GetMessage(&msg, 0, 0, 0);
		if (result > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);			
		}
		else
		{
			
			return result;
		}
	}
}


