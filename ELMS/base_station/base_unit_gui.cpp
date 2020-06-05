/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* This file contans definitions for functions that use the Win32 API to create
* GUI Windows
*/
#include "base_unit_gui.h"
#include <commctrl.h>

/*
* The following you tube reference was used to help writing this code. 
//https://www.youtube.com/watch?v=8GCvZs55mEM
//You tube reference called:
// Windows GUI Programming with C/C++ ( Win32 API ) | Parts 1 - 9 |
*/

//These are Win32 API handlers used.
HWND hList;
//HWND hWnd;
HWND settingsText, settingsText1, settingsText2, settingsText4;
HBITMAP hLogoImage, hLogoImage1, hLogoImage3, hLogoImage4, hLogoImage5;
HBITMAP hLogoImage6, hLogoImage7;
HWND hLogo, hLogo1;
//This char array holds the COM port. 
char comPort[50];
BOOL quit = false;
BOOL failure = false;


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
Third and Forth arguments contain addtional data that pertains to the message. 
LRESULT is an integer value returned as a response to the message. 
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

    /*Reference:https://www.cplusplus.com/forum/windows/176612/ 
	 * What this does is color the text in the child textbox red */
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wParam; // or obtain the static handle in some other way
		SetTextColor(hdcStatic, RGB(255, 0, 0)); // text color
		return (LRESULT)GetStockObject(WHITE_BRUSH);

	}
	break;

	case WM_COMMAND:

		if (LOWORD(wParam) == ID_SELF_DESTROY_BUTTON) {
			//if the select port button is pushed
			// reset buffer and comPort
			memset(buffer, '\0', 50);
			memset(comPort, '\0', 50);

			//get the list number that was selected
			DWORD64 dwSel = SendMessage(hList, LB_GETCURSEL, 0, 0);
			
			//if the selected is the top row, then no COM port was selected
			// because this is the instructions
			if (dwSel == 0)
			{
				MessageBox(NULL, "If no COM Port detected, please exit, otherwise select a COM Port", "COM Port", MB_ICONWARNING);
			}
			//if the selected is greater than 0, then this means the user selected a port
			if (dwSel > 0)
			{
				//retrieve the text
				SendMessage(hList, LB_GETTEXT, dwSel, (LPARAM)(LPCSTR) buffer);

				//copy it into the global string
				strcpy_s(comPort, buffer);

				//if the first character in comPort is not NULL, then a
				//comPort was selected and we change the flag to 1
				if (comPort[0] != 0)
				{
					flag = 1;
				}
				break;				
			}
			break;
		}

		//otherwise, if the Start button was pushed and the flag is now
		// 1, then we reset the flag to 0, and send a message to the window to close. 
		else if (LOWORD(wParam) == ID_CLOSE && flag == 1)
		{
			// reset flag to 0
			flag = 0;
			SendMessage(hWnd, WM_CLOSE, NULL, NULL );			
		}

		// otherwise, if start was push and the flag is 0 (no port selected)
		else if (LOWORD(wParam) == ID_CLOSE && flag == 0)
		{
			memset(buffer, '\0', 50);

			//send a message to get what the listbox cursor is pointing to. 
			DWORD64 dwSel1 = SendMessage(hList, LB_GETCURSEL, 0, 0);
			SendMessage(hList, LB_GETTEXT, dwSel1, (LPARAM)(LPCSTR)buffer);

			//if the text starts with an N, this means NULL so we close the window
			if (buffer[0] == 'N')
			{
				SendMessage(hWnd, WM_CLOSE, NULL, NULL);
			}
			// if the text returned has content, then the user is prompted to
			// make a selection. 
			else
			{
				MessageBox(NULL, "Before Start, select a COM port and Click Select Port Button", "Select a COM Port", MB_ICONWARNING);
			}
		}

		break;

	}
	return DefWindowProcW(hWnd, uMsg, wParam, lParam);
}

/*
=========================================================================
getPort1
This function creates the listbox Window.
//https://gist.github.com/Pilzschaf/d950a86042c37a9c8d1a8b9b5f082fff

===========================================================================
*/
BOOL getPort1(vector<string>* listOfPorts, string& name)
{
	HINSTANCE hInstance = GetModuleHandle(0);
	HWND hWnd;
	HWND hButton;
	HWND hButtonClose;
	WNDCLASSW wc = { 0 };
	MSG msg;

	wc.style = CS_HREDRAW | CS_VREDRAW;
	wc.lpfnWndProc = MessageHandler;
	wc.hCursor = LoadCursor(NULL, IDC_ARROW);
	wc.hInstance = hInstance;
	//This code gives the solid blue background color
	wc.hbrBackground = CreateSolidBrush(0xFF6633);
	wc.lpszClassName = L"SelectPort";

	if (!RegisterClassW(&wc))
		std::cout << "Failed to register" << std::endl;

	/*this creates the parent windows and sets it position on the screen
	  Arguments are:  Window class, Title of Window, Style of Window, X and Y
	  Position of Window, Width and Height of Window, remaining parameters are
	  set to NULL
	*/
	//loadImages();
	hWnd = CreateWindowW(L"SelectPort", L"WELCOME TO ELMS!", WS_OVERLAPPEDWINDOW |
		WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);
			
	
		loadImages();

		/*this creates the button in the window that the user will use to select the
		 * COM port.
		 * To create a button, use the windows class button. The second argument is
		 * the text present in the button, the 3rd argument is the style, the 4th
		 * and 5th arguments are the window coordinates. In this case we use 400 and
		 * 250. If the top left corner is (0,0), and the bottom right corner is
		 * (width of window, height of window), then (400, 250) will be placed 400
		 * pixel from the top left corner and down 250 pixels. The 6th and 7th
		 * arguments are the width and height of the button. The 8th argument is
		 * the handler of the parent window. The 9th argument is for HMenu ID_SELF_
		 * _DESTROY_BUTTON which we need to cast. The 10th argument is the hInstance,
		 * 11th is set to NULL.
		  */

			hButton = CreateWindowW(L"button", NULL, WS_TABSTOP | WS_THICKFRAME |
				WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON |BS_BITMAP, 412, 300, 118, 60, hWnd, (HMENU)ID_SELF_DESTROY_BUTTON,
				hInstance, 0);

			// send a message to the button to put the image map on its face
			SendMessageW(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage1);

			/*Create a button that will be used to close the window once the COM port
			has been selected */
			hButtonClose = CreateWindowW(L"button", NULL
				, WS_VISIBLE | WS_CHILD | BS_BITMAP | WS_THICKFRAME |
				WS_TABSTOP | BS_DEFPUSHBUTTON, 545, 300, 115, 60, hWnd, (HMENU)ID_CLOSE,
				NULL, NULL);
			SendMessageW(hButtonClose, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);

			// creates the list box control
			hList = CreateWindowEx(WS_EX_CLIENTEDGE, "listbox", "", WS_CHILD | WS_VISIBLE |
				WS_VSCROLL | ES_AUTOVSCROLL | LBS_EXTENDEDSEL | LBS_NOTIFY | WS_THICKFRAME, 435, 90, 200, 200,
				hWnd, (HMENU)ID_LISTBOX, 0, 0);

			//Build the contents of the list box
			//if the listOfPorts is empty, then tell the user
			if (listOfPorts->empty())
			{
				AddText_NoSerial(hWnd, hButtonClose);
				SetWindowTextW(hWnd, L"NO PORTS DETECTED!");
				SendMessage(hList, LB_ADDSTRING, 0, (LPARAM)"NO PORTS DETECTED!!");
			}

			//otherwise, convert the list of ports to messages that will be added to
			// the list in the listbox.
			if (!listOfPorts->empty())
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
		
		//this while loop continues until the window clsoes
		while (true)
		{

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
void AddText_NoSerial(HWND hWnd, HWND hButtonClose)
{
	settingsText = CreateWindowW(TEXT(L"STATIC"), TEXT(L"                        INSTRUCTIONS\r\n\r\n(1) Sorry, no COM ports were detected\
     \r\n(2) Click on the \"EXIT\" Button\r\n(3) The program will exit with an error."), WS_VISIBLE | WS_CHILD | WS_THICKFRAME,
		75, 90, 300, 100, hWnd, NULL, NULL, NULL);
	HDC hdcStatic = GetDC(settingsText);
	SendMessageW(hButtonClose, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage3);
	
}

/*
==============================================================================
AddText_SerialDetected
Adds instructions if serial ports are detected
==============================================================================
*/
void AddText_Serial(HWND hWnd)
{
	settingsText1 = CreateWindowW(TEXT(L"STATIC"), TEXT(L"                      INSTRUCTIONS\r\n\r\n(1) Click on a COM Port to Highlight it\
     \r\n(2) Click \"Select Port\" Button\r\n(3) Click \"Start\" Button\r\n(4) After start, press any key to quit"), WS_VISIBLE | WS_CHILD | WS_THICKFRAME,
		75, 90, 300, 125, hWnd, NULL, NULL, NULL);
	//HDC hdcStatic = GetDC(settingsText);
}


/*
===========================================================================
loadImages()
This loads the .bmp files that are used to color the buttons. It is 
important that they be placed in the same location as the .exe files or else
the images will not load. 
===========================================================================
*/
void loadImages()
{
	hLogoImage = (HBITMAP)LoadImageW(NULL, L"start.bmp", IMAGE_BITMAP, 205, 100, LR_LOADFROMFILE);
	hLogoImage1 = (HBITMAP)LoadImageW(NULL, L"selectionPort1.bmp", IMAGE_BITMAP, 205, 100, LR_LOADFROMFILE);
	hLogoImage3 = (HBITMAP)LoadImageW(NULL, L"exit.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
	hLogoImage4 = (HBITMAP)LoadImageW(NULL, L"confirm.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
	hLogoImage5 = (HBITMAP)LoadImageW(NULL, L"ignore.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
	hLogoImage6 = (HBITMAP)LoadImageW(NULL, L"quit.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
	hLogoImage7 = (HBITMAP)LoadImageW(NULL, L"continue.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
}

/*
====================
stringToWString
This function converts an input string into a wide string
which is commonly used in the Win32 API
Reference: https://stackoverflow.com/questions/27220/how-to-convert-stdstring-to-lpcwstr-in-c-unicode
====================
*/
std::wstring stringToWString(const std::string& s)
{
	int length;
	int stringLength = (int)s.length() + 1;
	length = MultiByteToWideChar(CP_ACP, 0, s.c_str(), stringLength, 0, 0);
	wchar_t* buffer = new wchar_t[length];
	MultiByteToWideChar(CP_ACP, 0, s.c_str(), stringLength, buffer, length);
	std::wstring result(buffer);
	delete[] buffer;
	return result;
}

/*
================
CloseHandler
This is the call back handler for the window that the user can use
to exit the program. 
================
*/
LRESULT CALLBACK CloseHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;

	case WM_COMMAND:
		switch (wp)
		{
		case ID_QUIT:
			quit = true;
			DestroyWindow(hWnd);
			break;

		case ID_CONTINUE:

			quit = false;
			DestroyWindow(hWnd);
			break;
		}

	break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);

	}
	return DefWindowProcW(hWnd, msg, wp, lp);
}

/*
===============
closeProgram 
This function opens a window that allows the user
to click a button and decide if they wish to 
continue or quit the program. 

Reference for unregistering a class:
https://stackoverflow.com/questions/18336605/proper-way-of-destroying-window-resources

Reference for preventing a class from being registered twice
https://stackoverflow.com/questions/37211360/class-already-exists
===============
*/
BOOL closeProgram()
{
	HINSTANCE hInstance = GetModuleHandle(0);
	HWND h;
	HWND hButtonQuit;
	HWND hButtonContinue;
	WNDCLASSW window = { 0 };
	MSG msg = {0};

	//we use a static name for the class and assign to a pointer
	static const wchar_t* className1;

	//only create the class and register it if it does not
	// already exist. This prevents failed to register class
	// errors. 
	if (nullptr == className1)
	{
		window.style = CS_HREDRAW | CS_VREDRAW;
		window.lpfnWndProc = CloseHandler;
		window.hCursor = LoadCursor(NULL, IDC_ARROW);
		window.hInstance = hInstance;
		//This code gives the solid blue background color
		window.hbrBackground = CreateSolidBrush(0xFF6633);
		window.lpszClassName = L"CloseProgram";

		if (!RegisterClassW(&window))
		{
			std::error_code err_code(GetLastError(), std::system_category());
			throw std::system_error(err_code);
		}
		className1 = L"CloseProgram";
	}

	//load the bitmap images
	loadImages();

	h = CreateWindowW(L"CloseProgram", L"Quit Program", WS_SYSMENU | WS_CAPTION | WS_MAXIMIZEBOX |
		WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 400, 400, NULL, NULL, hInstance, NULL);

	hButtonQuit = CreateWindowW(L"button", L"Quit", WS_TABSTOP | WS_THICKFRAME |
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_BITMAP, 77, 125, 110, 60, h, (HMENU)ID_QUIT,
		hInstance, 0);
	SendMessageW(hButtonQuit, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage6);

	hButtonContinue = CreateWindowW(L"button", L"Continue", WS_TABSTOP | WS_THICKFRAME |
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_BITMAP, 207, 125, 110, 60, h, (HMENU)ID_CONTINUE,
		hInstance, 0);
	SendMessageW(hButtonContinue, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage7);


	while (GetMessage(&msg, NULL, NULL, NULL))
	{

			TranslateMessage(&msg);
			DispatchMessage(&msg);
	}

	// Reference for unregistering a class:
	//https://stackoverflow.com/questions/18336605/proper-way-of-destroying-window-resources

	return quit;
}
/*
================
FailureHandler
This is the call back handler for the window that notifies the user
that network failure has occurred. The bool failure will be set to true
or false in this callback. 
Reference: https://stackoverflow.com/questions/31749262/setting-background-color-of-child-window-using-windows-handle
================
*/
LRESULT CALLBACK FailureHandler(HWND hWnd, UINT msg, WPARAM wp, LPARAM lp)
{
	switch (msg) {
	case WM_CLOSE:
		DestroyWindow(hWnd);
		break;

	break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;


	/*Reference:https://www.cplusplus.com/forum/windows/176612/ */
	case WM_CTLCOLORSTATIC:
	{
		HDC hdcStatic = (HDC)wp; // or obtain the static handle in some other way
		SetTextColor(hdcStatic, RGB(255, 0, 0)); // text color
		SetBkColor(hdcStatic, RGB(250, 250, 210));
		HBRUSH yellow = CreateSolidBrush(RGB(250, 250, 210));
		return (LRESULT)yellow;

	}
	break;

	case WM_COMMAND:
		switch (wp)
		{
		case ID_CONFIRM:
			//set failure to true.  The user confirmed it.
			failure = true;
			DestroyWindow(hWnd);
			break;

		case ID_UNCONFIRMED:
			// set failure to false because the user ignored it. 
			failure = false;
			DestroyWindow(hWnd);
			break;
		}

	case WM_TIMER:
		switch (wp)
		{
		case IDT_TIMER1:
			KillTimer(hWnd, IDT_TIMER1);
			DestroyWindow(hWnd);
			failure = true;
			break;
		}

		break;
	default:
		return DefWindowProcW(hWnd, msg, wp, lp);

	}
	return DefWindowProcW(hWnd, msg, wp, lp);
}


/*
===============
confirmNetworkFailure
This is the function that brings up a windows and
allows the user to confirm network failure or not

Reference: In order to bring this window to the
forefront, I used this reference and code snippet
from stackoverflow. 
https://stackoverflow.com/questions/916259/win32-bring-a-window-to-top/34414846

References used to avoid error of class failed to register.
These prevent the function from trying to register a class if it 
already exists. 
https://stackoverflow.com/questions/9761812/registering-a-window-class-in-the-win32api
https://stackoverflow.com/questions/37211360/class-already-exists

The window is also on a timer.  If the user does not respond
within 10 seconds, the window automatically closes and assumes
that the network failure is confirmed.  The reference for this is
https://docs.microsoft.com/en-us/windows/win32/winmsg/using-timers
==============
*/
BOOL confirmNetworkFailure(string str)
{
	//this is a temporary string that will be used to convert the
	//input string parameter to a wstring string and then ultimately
	// converted to a LPCWSTR type
	string tempString = str;
	tempString = "Network Failure - Confirm or Ignore\r\n\r\n\r\n" + str;

	//this calls a function that converts a string to a wstring
	std::wstring temp = stringToWString(tempString);
	LPCWSTR newString = temp.c_str();

	//declaring the necessary parts of the Window class
	HINSTANCE hInstance = GetModuleHandle(0);
	HWND h;
	HWND hButtonConfirm;
	HWND hButtonIgnore;
	HWND hText;
	WNDCLASSW window = { 0 };
	MSG msg = { 0 };

	//we use a static name for the class and assign to a pointer
	static const wchar_t* className;

	//only create the class and register it if it does not
	// already exist. This prevents failed to register class
	// errors. 
	 if (nullptr == className)
	{
		window.style = CS_HREDRAW | CS_VREDRAW;
		window.lpfnWndProc = FailureHandler;
		window.hCursor = LoadCursor(NULL, IDC_ARROW);
		window.hInstance = hInstance;
		//This code gives the solid blue background color
		window.hbrBackground = CreateSolidBrush(0xFF6633);
		window.lpszClassName = L"FailureProgram";

		if (!RegisterClassW(&window))
		{
			std::error_code err_code(GetLastError(), std::system_category());
			throw std::system_error(err_code);
		}
		className = L"FailureProgram";
	}

	h = CreateWindowW(L"FailureProgram", L"Confirm Network Failure", WS_POPUPWINDOW | WS_CAPTION | WS_MAXIMIZEBOX|
		WS_VISIBLE, CW_USEDEFAULT, CW_USEDEFAULT, 800, 600, NULL, NULL, hInstance, NULL);

	//this is the code snippet from the reference above that was used
	//to bring the window to the foreground process
	HWND hCurWnd = ::GetForegroundWindow();
	DWORD dwMyID = ::GetCurrentThreadId();
	DWORD dwCurID = ::GetWindowThreadProcessId(hCurWnd, NULL);
	::AttachThreadInput(dwCurID, dwMyID, TRUE);
	::SetWindowPos(h, HWND_TOPMOST, 100, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE);
	::SetWindowPos(h, HWND_NOTOPMOST, 100, 0, 0, 0, SWP_SHOWWINDOW | SWP_NOSIZE | SWP_NOMOVE);
	::SetForegroundWindow(h);
	::SetFocus(h);
	::SetActiveWindow(h);
	::AttachThreadInput(dwCurID, dwMyID, FALSE);

	//bitmap images are loaded and available for the buttons
	loadImages();

    //load the text and buttons
	hText = settingsText4 = CreateWindowW(TEXT(L"STATIC"), TEXT(newString), WS_VISIBLE | WS_CHILD | WS_THICKFRAME | SS_CENTER
		, 100, 70, 555, 100, h, NULL, NULL, NULL);

	hButtonConfirm = CreateWindowW(L"button", L"Confirm", WS_TABSTOP | WS_THICKFRAME |
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_BITMAP, 260, 200, 100, 60, h, (HMENU)ID_CONFIRM,
		hInstance, 0);
	SendMessageW(hButtonConfirm, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage4);

	hButtonIgnore = CreateWindowW(L"button", L"Ignore", WS_TABSTOP | WS_THICKFRAME | 
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_BITMAP, 390, 200, 100, 60, h, (HMENU)ID_UNCONFIRMED,
		hInstance, 0);
	SendMessageW(hButtonIgnore, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage5);

	SetTimer(h, IDT_TIMER1, 10000, (TIMERPROC) NULL);

	//this while loop continues while the windows waits for input from the user 
	//via clicking on a button
	while (GetMessage(&msg, NULL, NULL, NULL))
	{

		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return failure;
}