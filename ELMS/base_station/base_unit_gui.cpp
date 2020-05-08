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

	AppendMenu(hSubMenu, MF_STRING, CHANGE_TITLE, "Change Title");

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
