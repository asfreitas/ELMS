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
LPCSTR temp;
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

	CreateWindowW(L"myWindowClass", L"My Window", WS_OVERLAPPEDWINDOW | WS_VISIBLE | WS_MAXIMIZEBOX, 100, 100, 500, 500, NULL, NULL, NULL, NULL);
	MSG msg = { 0 };

	while (GetMessage(&msg, NULL, NULL, NULL))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

}

void AddMenus(HWND handle)
{
	HMENU hMenu = CreateMenu();
	HMENU hFileMenu = CreateMenu();
	HMENU hSubMenu = CreateMenu();

	AppendMenuA(hSubMenu, MF_STRING, CHANGE_TITLE, "Change Title");

	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_NEW, "New");
	AppendMenu(hFileMenu, MF_POPUP, (UINT_PTR) hSubMenu, "Open Submenu");
	AppendMenu(hFileMenu, MF_SEPARATOR, NULL, NULL);
	AppendMenu(hFileMenu, MF_STRING, FILE_MENU_EXIT, "Exit");



	AppendMenu(hMenu, MF_POPUP, (UINT_PTR) hFileMenu, "File");
	AppendMenu(hMenu, MF_STRING, NULL, "Help");
	SetMenu(handle, hMenu);

}

void AddControls(HWND hWnd)
{
	CreateWindowW(L"static", L"Enter Text In The Box Below ", WS_VISIBLE | WS_CHILD | WS_BORDER | SS_CENTER, 200, 100, 100, 50, hWnd, NULL, NULL, NULL);
	hEdit = CreateWindowW(L"edit", L" ", WS_VISIBLE | WS_CHILD| WS_BORDER | ES_MULTILINE | ES_AUTOVSCROLL, 200, 152, 100, 50, hWnd, NULL, NULL, NULL);

}


LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
	switch (uMsg) {
	case WM_CLOSE:
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_COMMAND:
		
		if (LOWORD(wParam) == ID_SELF_DESTROY_BUTTON) {
			if (SendMessage(hList, LB_GETSEL, 0, 0) > 0) {
				SendMessage(hWnd, LB_GETTEXT, 0, lParam);
				temp = "COM8";
				std::cout << "Here is the value of temp " << temp << std::endl;
				return 0;
			}
			else if (SendMessage(hList, LB_GETSEL, 1, 0) > 0) {
				std::cout << "Middle selected" << std::endl;
				return 0;
			}
			else if (SendMessage(hList, LB_GETSEL, 2, 0) > 0) {
				std::cout << "Middle selected" << std::endl;
				return 0;
			}
			else if (SendMessage(hList, LB_GETSEL, 3, 0) > 0) {
				std::cout << "Middle selected" << std::endl;
				return 0;
			}
		}
		break;

	}
	return DefWindowProc(hWnd, uMsg, wParam, lParam);
}
//https://gist.github.com/Pilzschaf/d950a86042c37a9c8d1a8b9b5f082fff
bool getPort(vector<string> *listOfPorts, LPCSTR &name)
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

<<<<<<< Updated upstream
	hWnd = CreateWindowW(L"WINAPITest", L"WinAPI Tutorial", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	hButton = CreateWindowA("button", "Selbstzerstörung", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 70, 300, 50, hWnd, (HMENU)ID_SELF_DESTROY_BUTTON, hInstance, 0);
	hList = CreateWindowExA(WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL, 400, 40, 150, 200, hWnd, (HMENU)ID_LISTBOX, 0, 0);

	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"first");
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"second");
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"last");
||||||| merged common ancestors
	hWnd = CreateWindowW(L"WINAPITest", L"WinAPI Tutorial", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	hButton = CreateWindow("button", "Selbstzerstörung", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 70, 300, 50, hWnd, (HMENU)ID_SELF_DESTROY_BUTTON, hInstance, 0);
	hList = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL, 400, 40, 150, 200, hWnd, (HMENU)ID_LISTBOX, 0, 0);

	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"first");
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"second");
	SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"last");
=======
	hWnd = CreateWindowW(L"SELECT_PORT", L"Select a COM port", WS_OVERLAPPEDWINDOW | WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, 0, 0, hInstance, 0);
	hButton = CreateWindowW(L"button", L"Select Port and Click This Button", WS_TABSTOP | WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON, 10, 70, 300, 50, hWnd, (HMENU)ID_SELF_DESTROY_BUTTON, hInstance, 0);
	hList = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE | WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL | LBS_NOTIFY, 400, 40, 150, 200, hWnd, (HMENU)ID_LISTBOX, 0, 0);
	if (listOfPorts->empty())
	{
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"No Ports Detected");
		Sleep(2);
		exit(1);
	}
	else
	{
		for (size_t i = 0; i < listOfPorts->size(); i++)
		{
			string str = listOfPorts->at(i);
			// convert str to c-string
			const char* temp = str.c_str();
			LPCTSTR portname = (LPCTSTR)temp;
			SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)portname);
		}
	//https://stackoverflow.com/questions/42438135/c-winapi-listbox-getting-selected-item-using-lb-getsel-lb-getcursel	
	}
	name = temp;
	printf("Here is the value of name in the Window: %s\n", name);
	//SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"first");
	//SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"second");
	//SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"last");
>>>>>>> Stashed changes

	//std::cout << "Created window" << std::endl;

	while (true) {
		BOOL result = GetMessage(&msg, 0, 0, 0);
		if (result > 0) {
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
		else {
			return result;
		}
	}
}