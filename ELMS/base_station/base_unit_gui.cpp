/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* This file contans definitions for functions that use the Win32 API to create
* GUI Windows
*/
#include "base_unit_gui.h"
#include <commctrl.h>

//https://www.youtube.com/watch?v=8GCvZs55mEM
//You tube reference called:
// Windows GUI Programming with C/C++ ( Win32 API ) | Part -1 | Creating a window

//These are Win32 API handlers used.
HWND hEdit;
HWND hList;
HWND settingsText, settingsText1, settingsText2;
static HBRUSH hBrush = CreateSolidBrush(RGB(230, 230, 230));
HBITMAP hLogoImage, hLogoImage1;
HWND hLogo, hLogo1;

//This char array holds the COM port. 
char comPort[50];

/*
TOP PART OF THIS FILE IS TUTORIAL -- BOTTOM HALF HAS PROGRAM CODE

*/

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
            /*This is a predefined function that gets the text inside the
			 * control. First argument is the handler of the control. The 
			 * second is where we want to store the text that is received, and
			 * the third argument is the maximum length */
			GetWindowTextW(hEdit, text, 100);

			/* First argument is the handler of the window that we want to
			 *  change the text. In this case, it is the parent window. The 
			 *  second argument is the text.
			 */
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
	return DefWindowProcW(hWnd, msg, wp, lp);
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

	AppendMenuA(hSubMenu, MF_STRING, NULL, "SumMenu Item");

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
to enter text. The parameter is the window handler. 
==============================================================================
*/
void AddControls(HWND hWnd)
{
	/* Win32 api has predefined classes of static and edit. First argument is the
	 * class, second argument is the name of the window or the text inside the 
	 * control. Third argument is style. Important to make sure that you use 
	 * WS_VISIBLE | WS_CHILD because a control is a child of the parent window.
	 * WS stands for Window Style. SS means static style which gives the 
	 * alignment of the text. 
	 * The 4th and 5th arguments are the x and y coordinates in the window. 
	 * The top left coordinate is (0,0), The bottom left coordinate is (0, size of parent)
	 * The top right is (size of parent, 0) and the bottom right is (size of parent, size of 
	 * parent). The 6th and 7 arguments are the width and the height. In this case,
	 * the width and height are 100 and 50. The 8th argument is the parent. hWnd 
	 * is the parent widow of this static box. 9th argument is any menu which in
	 * this case is NULL, 10th argument is the hInstance, but this is a child so
	 * it is a NULL.  The last argument is another argument which in this case
	 * is also not needed. 
	*/
	CreateWindowW(L"static", L"Enter Text In The Box Below ", WS_VISIBLE | WS_CHILD |
		WS_BORDER | SS_CENTER, 200, 100, 100, 50, hWnd, NULL, NULL, NULL);

	/* edit is the predined class for windows api which allows the user to 
	 * edit in the window. Note that 152 was used in this child because the 
	 * first box is (200, 100), so we want it to start a couple of pixels below
	 * the top box. The width and height are the same as the box above. The
	 * styles of ES_MULTILINE and AUTOVSCROLL allows the user to type in 
	 * multilines and also for the window to scroll.
	 * Whenever the CreateWindowW function is called, a handler is returned. 
	 * This handler is stored in hEdit. 
	 */
	hEdit = CreateWindowW(L"edit", L" ", WS_VISIBLE | WS_CHILD | WS_BORDER |
		ES_MULTILINE | ES_AUTOVSCROLL, 200, 152, 100, 50, hWnd, NULL, NULL, NULL);

	/* To create a button, use the windows class button. The second argument is 
	   the text present in the button, the 3rd argument is the style, the 4th 
	   and 5th arguments are the window coordinates. In this case we use 204 and
	   100 because the area above was 152 + 50 and then we add 2 pixels more for
	   spacing. The 6th and 7th arguments are the width and height which are 
	   kept the same. 8th argument is the handler of the parent window. The 9th
	   argument is for HMenu, CHANGE_TITLE but we also need to cast it.
	   The 10th and 11th arguments are NULL. 
	 */
	CreateWindowW(L"button", L"Change title", WS_VISIBLE | WS_CHILD, 204, 100, 100, 50,
		hWnd, (HMENU)CHANGE_TITLE, NULL, NULL);
}

/*
Tutorial is above -- Program code is below. 
================================================================================
===============================================================================
================================================================================
*/

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

First Argument -- Window handler
Second Argument -- Message which have been sent
Third Argument -- 
Fourth Argument -- 
=============================================================================
*/
LRESULT CALLBACK MessageHandler(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	// a flag is used so that the user cannot exit via the close button until
	// they have selected a port. 
	static int flag = 0;
	char buffer[50];
	switch (uMsg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_CREATE:
		//AddText(hWnd);
		//loadImages();

	/*case WM_PAINT: {
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(hWnd, &ps);
		HDC hdc_x = CreateCompatibleDC(NULL);
		HBITMAP hBitmap = (HBITMAP)LoadImage(NULL, "C:\\Users\\DEBBIE\\source\\repos\\ELMS base gui\\x64\\red.bmp", IMAGE_BITMAP,
			0, 0, LR_LOADFROMFILE);
		SelectObject(hdc_x, hBitmap);

		RECT rect;
		GetWindowRect(hWnd, &rect);
		BitBlt(hdc, 0, 0, rect.right - rect.left, 400, hdc_x, 0, 0, SRCCOPY);
		EndPaint(hWnd, &ps);
	}*/

	/*case WM_CTLCOLORSTATIC:
	{
		if (settingsText == (HWND)lParam)
		{
			//OR if the handle is unavailable to you, get ctrl ID

			DWORD CtrlID = GetDlgCtrlID((HWND)lParam); //Window Control ID
			if (CtrlID == IDC_STATIC1) //If desired control
			{
				HDC hdcStatic = (HDC)wParam;
				SetTextColor(hdcStatic, RGB(0, 0, 0));
				SetBkColor(hdcStatic, RGB(255, 0, 0));
				return (INT_PTR)hBrush;
			}
		}
	}*/
		/*Reference:https://www.cplusplus.com/forum/windows/176612/ */
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam; // or obtain the static handle in some other way
		SetTextColor(hdcStatic, RGB(255, 0, 0)); // text color
		//SetBkColor(hdcStatic, RGB(255, 255, 224));
		return (LRESULT)GetStockObject(WHITE_BRUSH);
		//return (INT_PTR)hBrush;
		//return (INT_PTR)CreateSolidBrush(RGB(255, 255, 255));
	}
	break;

	case WM_COMMAND:

		if (LOWORD(wParam) == ID_SELF_DESTROY_BUTTON) {
			//if the select port button is pushed
			//char buffer[50];
			// reset buffer and comPort
			memset(buffer, '\0', 50);
			memset(comPort, '\0', 50);
			//get the list number that was selected
			DWORD64 dwSel = SendMessage(hList, LB_GETCURSEL, 0, 0);
			if (dwSel > 0)
			{
				//retrieve the text
				SendMessage(hList, LB_GETTEXT, dwSel, (LPARAM)(LPCSTR) buffer);
				//copy it into the global string
				strcpy_s(comPort, buffer);
				if (comPort[0] != 0)
				{
					flag = 1;
				}
				break;				
			}
			break;
		}
		else if (LOWORD(wParam) == ID_CLOSE && flag == 1)
		{
			// reset flag to 0
			flag = 0;
			SendMessage(hWnd, WM_CLOSE, NULL, NULL );			
		}
		else if (LOWORD(wParam) == ID_CLOSE && flag == 0)
		{
			memset(buffer, '\0', 50);
			DWORD64 dwSel1 = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_GETTEXT, dwSel1, (LPARAM)(LPCSTR)buffer);
			if (buffer[0] == 'N')
			{
				SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			}
			else
			{
				MessageBox(NULL, "Before Exit, select a COM port and Click Select Port Button", "Select a COM Port", MB_ICONWARNING);
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
	HWND hButtonClose;
	WNDCLASS wc = { 0 };
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MessageHandler;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	wc.hbrBackground = CreateSolidBrush(0xFF6633);
	//wc.hbrBackground = GetSysColorBrush(COLOR_3DFACE);// (GetSysColorBrush(COLOR_3DFACE);//(HBRUSH)GetStockObject(LTGRAY_BRUSH);
	wc.lpszClassName = "SelectPort";

	if (!RegisterClass(&wc))
		std::cout << "Failed to register" << std::endl;

	/*this creates the parent windows and sets it position on the screen
	  Arguments are:  Window class, Title of Window, Style of Window, X and Y 
	  Position of Window, Width and Height of Window, remaining parameters are
	  set to NULL
	*/
	//loadImages();
	hWnd = CreateWindowW(L"SelectPort", L"Select a Port", WS_OVERLAPPEDWINDOW |
		WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT,800, 600, NULL, NULL, hInstance, NULL);
	loadImages();
	/*this creates the button in the window that the user will use to select the
	 * COM port. */

	hButton = CreateWindowW(L"button", NULL, WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_BITMAP | WS_BORDER , 400, 250, 100, 60, hWnd, (HMENU)ID_SELF_DESTROY_BUTTON,
		hInstance, 0);
	SendMessageW(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage1);

	/*Create a button that will be used to close the window once the COM port 
	  has been selected */
	hButtonClose = CreateWindowW(L"button", NULL
		, WS_VISIBLE | WS_CHILD | BS_BITMAP| WS_BORDER |
		WS_TABSTOP | BS_DEFPUSHBUTTON , 502, 250, 100, 60, hWnd, (HMENU)ID_CLOSE,
		NULL, NULL);
	SendMessageW(hButtonClose, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);


	//hLogo = CreateWindowW(L"static", L"Exit", WS_VISIBLE | WS_CHILD | SS_BITMAP, 100, 50, 98, 500, hWnd, NULL, NULL, NULL);
	//SendMessageW(hLogo, STM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);

	hList = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE |
		WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL | LBS_NOTIFY | WS_THICKFRAME, 400, 40, 200, 200, 
		hWnd, (HMENU)ID_LISTBOX, 0, 0);

	//Build the contents of the list box
	//if the listOfPorts is empty, then tell the user
	if (listOfPorts->empty())
	{
		AddText_NoSerial(hWnd);
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"NO PORTS DETECTED!!");
	}

	//otherwise, convert the list of ports to messages that will be added to
	// the list in the listbox.
	if(!listOfPorts ->empty())
	{
		//Build the contents of the list box
		SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"Select a COM Port");
		AddText_Serial(hWnd);
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
	return 0;
}
/*
==========================================================================
* AddText

* Predined class for text is static. Class name static is not sensitive. Second
* Argument is the text in the static control.  This is usually a label. Third
* argument is style. You must "Or" the two arguments here. The next two arguments
* are the x and y coordinates. Next two arguments are the width and height, next
* arguments is the parent window handler, next is HMenu if there is one, then
* the instance but since this is a child we put NULL, and lastly a parameter 
* which is not needed, so it is kept as NULL.
References:
http://forums.codeguru.com/showthread.php?105498-Multiline-in-Static-Text
https://stackoverflow.com/questions/45653034/c-win32-change-static-color

===============================================================================
*/
void AddText_NoSerial(HWND hWnd)
{
	settingsText = CreateWindowW(TEXT(L"STATIC"), TEXT(L"INSTRUCTIONS:\r\n\r\n(1) Sorry, no COM ports were detected\
     \r\n(2) Click on the \"EXIT\" Button\r\n(3) The program will exit with an error."), WS_VISIBLE | WS_CHILD | WS_THICKFRAME,
		10, 40, 300, 100, hWnd, NULL, NULL, NULL);
	HDC hdcStatic = GetDC(settingsText);
	
}

/*
==============================================================================
AddText_SerialDetected
Adds instructions if serial ports are detected
==============================================================================
*/
void AddText_Serial(HWND hWnd)
{
	settingsText1 = CreateWindowW(TEXT(L"STATIC"), TEXT(L"INSTRUCTIONS:\r\n\r\n(1) Click on a COM Port to Highlight it\
     \r\n(2) Click \"Select Port\" Button\r\n(3) Click \"Exit\" Button"), WS_VISIBLE | WS_CHILD | WS_THICKFRAME,
		10, 40, 300, 100, hWnd, NULL, NULL, NULL);
	HDC hdcStatic = GetDC(settingsText);
}


/*
===========================================================================
loadImages()

*/
void loadImages()
{
	hLogoImage = (HBITMAP)LoadImageW(NULL, L"red.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
	hLogoImage1 = (HBITMAP)LoadImageW(NULL, L"blue.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
}


