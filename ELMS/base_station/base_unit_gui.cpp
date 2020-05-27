/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* This file contans definitions for functions that use the Win32 API to create
* GUI Windows
*/
#include "base_unit_gui.h"
#include <commctrl.h>

//https://www.youtube.com/watch?v=8GCvZs55mEM
//You tube reference called:
// Windows GUI Programming with C/C++ ( Win32 API ) | Parts 1 - 9 |

//These are Win32 API handlers used.
HWND hList;
HWND settingsText, settingsText1, settingsText2;
static HBRUSH hBrush = CreateSolidBrush(RGB(230, 230, 230));
HBITMAP hLogoImage, hLogoImage1;
HWND hLogo, hLogo1;

//This char array holds the COM port. 
char comPort[50];

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

    /*Reference:https://www.cplusplus.com/forum/windows/176612/ */
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

===========================================================================
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

	hButton = CreateWindowW(L"button", NULL, WS_TABSTOP |
		WS_CHILD | WS_VISIBLE | BS_DEFPUSHBUTTON | BS_BITMAP | WS_BORDER , 400, 250, 100, 60, hWnd, (HMENU)ID_SELF_DESTROY_BUTTON,
		hInstance, 0);

	// send a message to the button to put the image map on its face
	SendMessageW(hButton, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage1);

	/*Create a button that will be used to close the window once the COM port 
	  has been selected */
	hButtonClose = CreateWindowW(L"button", NULL
		, WS_VISIBLE | WS_CHILD | BS_BITMAP| WS_BORDER | 
		WS_TABSTOP | BS_DEFPUSHBUTTON , 502, 250, 100, 60, hWnd, (HMENU)ID_CLOSE,
		NULL, NULL);
	SendMessageW(hButtonClose, BM_SETIMAGE, IMAGE_BITMAP, (LPARAM)hLogoImage);


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
This loads the two .bmp files that are used to color the buttons. It is 
important that they be placed in the same location as the .exe files
===========================================================================
*/
void loadImages()
{
	hLogoImage = (HBITMAP)LoadImageW(NULL, L"red.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
	hLogoImage1 = (HBITMAP)LoadImageW(NULL, L"blue.bmp", IMAGE_BITMAP, 200, 100, LR_LOADFROMFILE);
}


