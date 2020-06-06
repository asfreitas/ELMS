// Mobile simulator for ELMS project
//	K.T. Kilty
//	December 31, 2019  Rev. 1.0
//
//
//
// Outline of this program
// It sets up communications attributes
// Set up a console, title it, create a scrolling area for messages,
// and labels data fields
// Enter a while loop to send simulated notifications from a script
// and process incoming serial stream from Base Pico900
// Modem (Base is master). Also looks for hotkey input to:
//	'x'-exit program
//
// If a notification comes from any mobile unit the loop performs the
// following functions
//
// 1. Opens the simulation file
// 2. Once per second sends a scripted notification
// 3. Checks for incoming collision notices from base.
// 4. Logs incoming notices to a file
// 5. Places this notice into a screen message
//
// Data structures involved
//
// Notification from mobile to base
// Commas separated variables, all ASCII chars
// These data fields, with the exception of unit no. are all
// available from a GPS "RMC" sentence, thus easy for the
// mobile microcontroller to handle
//
// Starts with a '$' and ends with a '*' character to aid in asynchronous processing
//
// unit number (1)
// time stamp
// latitude
// longitude
// speed over ground
// bearing
//
// Notification from base to affected units
//
// Comma separated, ASCII stream
// A number of these are calculated by the base unit for reason
// that the base unit is a faster, more powerful processor
//
// unit number 1 to unit number 2
// time stamp
// speed of unit
// approximate distance to unit
// bearing to unit
//
// The serial port handler (serialport.c) came from A GitHub project
// Some of the screen handling routines are based on examples from MicroSoft
// Windows Console routines.
#define _CRT_SECURE_NO_WARNINGS
#include <stdio.h>
#include <conio.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>
#include "serialport.h"

//defines


#define FIELDS 5	// there are five fields in the message a mobile unit must process

// ---------------------------------------------
// 70
// --------- DATA Structure for unit storage ----
//typedef struct {
//int unit_no;
//char time_s[10];
//int speed;
//int distance;
//int bearing;
//} UNIT;


// prototypes
void closeSerialPort(HANDLE h);
HANDLE hStdout;
CONSOLE_SCREEN_BUFFER_INFO csbiInfo;

void SetHorizontalLine(HANDLE hOut, int y)
{
int c;
COORD setcoords;
	setcoords.X = 0;
	setcoords.Y = y;
   SetConsoleCursorPosition( hOut, setcoords);
	for(c=0; c<80; c++) printf("=");
}

void SetString(HANDLE hOut, int x, int y, const char *s)
{
COORD setcoords;
	setcoords.X = x;
	setcoords.Y = y;
   SetConsoleCursorPosition( hOut, setcoords);
	printf("%s", s);
}


void InitiateScreen(HANDLE hOut)
{
// set labels

	SetString(hOut, 10, 5, "UTC Time:");
//	SetString(hOut, 10, 6, "Latitude:");
	SetString(hOut, 10, 7, "Time to process");

	SetString(hOut, 10, 8, "Notification:");
//	SetString(hOut, 10, 10, "Altitude:");


//	SetString(hOut, 40, 5, "Main Battery:");
//	SetString(hOut, 40, 6, "Logic Battery:");
//	SetString(hOut, 40, 8, "Ext Temp:");
//	SetString(hOut, 40, 9, "Acc Temp:");
//	SetString(hOut, 40, 10, "Prs Temp:");

	SetHorizontalLine(hOut, 11);
	SetString(hOut, 15, 12, "Messages Below; Information Above; 'x' to exit");
//	SetString(hOut, 10, 13, "Hotkeys: s=swap power a=arm  d=drop package c=get signal level");

	SetHorizontalLine(hOut, 13);
}

void PlaceInfo(HANDLE hOut, char *s, int n, int w, int col, int row)
{
char * info;
info = s + (n-1)*w;
	SetString(hOut, col, row, info);
}


int ScrollScreenMessages(HANDLE hOut, char *msg, int nEntries)
{
   SMALL_RECT srctScrollRect, srctClipRect;
   CHAR_INFO chiFill;
   COORD coordDest, setcoords;
   // Let's make scrolling rectangle rows beyond 6 of the
   // screen buffer, and allow no more than 30 such lines for
	// messages.



   srctScrollRect.Top = 16;
   srctScrollRect.Bottom = 36;
   srctScrollRect.Left = 0;
   srctScrollRect.Right = csbiInfo.dwSize.X - 1;

   // The destination for the scroll rectangle is one row down.

   coordDest.X = 0;
   coordDest.Y = 17 + nEntries;

   // The clipping rectangle is the same as the scrolling rectangle.
   // The destination row is left unchanged.

   srctClipRect = srctScrollRect;

   // Fill the bottom row with blanks.
   chiFill.Attributes = 0;
   chiFill.Char.AsciiChar = (char)' ';

   // Scroll up one line.

   if(!ScrollConsoleScreenBuffer(
       hOut,         // screen buffer handle
       &srctScrollRect, // scrolling rectangle
       &srctClipRect,   // clipping rectangle
       coordDest,       // top left destination cell
       &chiFill))       // fill character and color
   {
       printf("ScrollConsoleScreenBuffer failed %d\n", GetLastError());
       return 1;
   }
	// print the message at x=0, y=16
	setcoords.X=0;
	setcoords.Y=16;
   SetConsoleCursorPosition( hOut, setcoords);
	printf("%s\n",msg);
return(nEntries++);

}


int main(int argc, char *argv[])
{
system("@cls||clear"); // https://stackoverflow.com/questions/2347770/how-do-you-clear-the-console-screen-in-c
DWORD bytesWritten,bytesRead,messagetick,currenttick,delay;
FILE *script, *logp;
char *msg;
char readbuffer[100];	 // buffer for serial incoming data
char messagebuffer[100]; // buffer for scrolling message area
char info[20];
char c,command;
int field,b,n;	// which data field are we reading?

// Ring buffer and pointers
int readnext=0;		// next available spot for character from serial port
int processnext=0;	// next character to be processed by the state machine
char ring[128];		// storage for upto 128 characters at a time

// field storage
char fielddata[5][12];	// 5 data fields of maximum length 11 characters
int fieldnext[5]; 	// points to next empty spot in field buffer
char label[5][12]={"Unit No.: "," Time: "," Speed: "," Distance: "," Bearing: "};
int data_to_send=FALSE;

char* line = NULL;
size_t line_size = 0;
int outDelay;
char* image;
COORD setcoords;
int i,nEntries=0;

// set up serial port 
	HANDLE h = openSerialPort(argv[1],B9600,one,off);

// set up the console

   hStdout = GetStdHandle(STD_OUTPUT_HANDLE);

   if (hStdout == INVALID_HANDLE_VALUE)
   {
       printf("GetStdHandle failed with %d\n", GetLastError());
       return 1;
   }
// set title
	if (!SetConsoleTitle("Simulation Mobile Station for ELMS -- x to exit") ) {
		printf("Console title failed?\n");
	}

   // Get the screen buffer size.

   if (!GetConsoleScreenBufferInfo(hStdout, &csbiInfo))
   {
       printf("GetConsoleScreenBufferInfo failed %d\n", GetLastError());
       return 1;
   }

	InitiateScreen(hStdout);

// open a log file

	logp = fopen("mobile.log","a");

// open a simulation script file

	if ((script = fopen(argv[2],"r")) == NULL) {
		printf("Can't open script file.\n");
		return(0);
	}
// start processing incoming serial stream
	msg=messagebuffer;
// set current tick counter
	currenttick=GetTickCount();


// --------------Main Loop -----------------------
//while loop to process stream of data
// -----------------------------------------------

	while( 1 ) {

// ================================================
// Top Priority
// has any notice come from the base?
// ================================================

	bytesRead = readFromSerialPort(h,readbuffer,99);
	readbuffer[bytesRead]=0;
	if (bytesRead>0) {
	for (i=0; i<bytesRead; i++) {
	c=readbuffer[i];
	putc(c,logp);
	ring[readnext]=c;
	readnext++; // boost ring pointer for next character
	readnext = readnext & 0x007F; // modulo ring size
	} // end of for loop on characters read
	} // end if stmt on bytes read from serial port

// =================================================
// Second priority
// do we need to process any received I/O?
// =================================================

	while (readnext != processnext) {
	c = ring[processnext];
	processnext++;
	processnext = processnext & 0x007F;

	// let's do something with the message string
	switch (c) {
	case  '$': // begins a notification
		msg = messagebuffer; // make a string of what actually comes in
		field = 0; // points to the first data storage location
		for(i=0; i<FIELDS; i++) { // terminating character in each field
			fielddata[i][0]='\0';
			fieldnext[i]=0; // next location of each field is zero
		} // end for loop
		{ *msg=c; msg++; }
		break;
	case '*' : // ends a notification
		{ *msg=c; msg++; *msg='\0';}

		for(i=0; i<FIELDS; i++) { // write terminating character in each field
			fielddata[i][fieldnext[i]]='\0';

		} // end for loop
		delay = GetTickCount();
  	        delay = delay - messagetick;
  	        sprintf(info,"%5d milliseconds",delay);
 	        PlaceInfo(hStdout, info, 1, 0, 23, 8);
		PlaceInfo( hStdout, (char *)fielddata, 2, 12, 19, 5);
		b=0;
		for(i=0; i<FIELDS; i++) {
		strcpy(&messagebuffer[b],&label[i][0]);
		b=strlen(&messagebuffer[0]);
		strcpy(&messagebuffer[b],&fielddata[i][0]);
		b=strlen(&messagebuffer[0]);
		}

		nEntries=ScrollScreenMessages(hStdout, messagebuffer, nEntries);

		break;
	case ',' :  // comma delimits a field
		field++;
		{ *msg=c; msg++; }
		break;
	default: // this is just some ASCII digit, store it -- strip blanks, cntrl, etc
		if (isgraph(c)) {
	 	fielddata[field][fieldnext[field]]=c;
		fieldnext[field]++;
		{ *msg=c; msg++; }}
		break;
	}  // end of switch
	} // end of while loop over characters processed

// ======================================================
// Third priority.
// time to send simulated script message?
// ======================================================
	if(data_to_send == FALSE)
	{
	if (getline(&line, &line_size, script) != -1)
		{
			outDelay = atoi(strtok(line, " "));
			image = strtok(NULL, " ");
			currenttick=GetTickCount();
			data_to_send=TRUE;
		} 
	}
		
	//	}
//	// get data from simulation script
  if (data_to_send == TRUE) {
	messagetick= GetTickCount();

	int diff = messagetick - currenttick;
    if (messagetick-currenttick>outDelay) {
 	// send this out to Base
	strcat(image,"\r\0");
	b=strlen( image);
	writeToSerialPort(h, image, b);
		data_to_send=FALSE;
    }
  }


// ======================================================
// Fourth Priority
// check for keyboard input
// ======================================================

	if ( _kbhit() ) {
		command = _getch();
		switch (command) {

		case 'x':
			closeSerialPort(h);
			fclose(logp);
   			 return 0;
		default : break;
		} // end switch
	} // end if kbhit

	} // end of while loop to process stream

} // end of main()
