/*
References:
 * https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
 * https://github.com/xanthium-enterpreises/Serial-Programming-Win32API-C
 * https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtok-s-strtok-s-l-wcstok-s-wcstok-s-l-mbstok-s-mbstok-s-l?view=vs-2019
 * Reference used to get the current time and date to name the files
 * https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
*/

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "serialport.hpp"
#include <vector>
#include "base_unit.h"
#include <iostream>
#include <fstream>
# include <string>
#include <ctime>
#include <time.h>
#include "port.h"
using std::cout;
using std::endl;
using std::ofstream;
using std::ios;
using std::string;


/* reference for the following -- this was an issue if you are using Visual Studio
 * https://stackoverflow.com/questions/22210546/whats-the-difference-between-strtok-and-strtok-r-in-c/22210711       */

//#ifdef _MSC_VER
//#define strtok_r strtok_s
//#endif

 /* A note about how to compile this program using a makefile in mingw
  * change directory to your files. Then run: mingw32-make all
  * to clean: mingw32-make clean */


int main()
{
	// declare a base class object
	Base_Unit b;
    
	string fileName;
	string message;
	string path = b.getPathToLogs();

	//This function is called to create the log directories that will be 
	// used in the program...if they do not already exist. 
	b.createFolder(path);

	// this function call assigns a file name based on the date and time and
	// add what type of file it is.  0 = incoming message, 1 = alert, 2 = network failure
	// 3 = misc errors
	b.createFileName(&fileName, 0);
	
	//concatenate the path to the message to the front of your file name. 
	// this ensures that the log file (whatever type) will be stored in the
	// correct folder. 
	fileName = b.getPathToMessages() + fileName;

	//declare an outfile and open it using the name created above
	ofstream file;
	file.open(fileName, std::ios::out | std::ios::app);

	//declare iterators that will be used
	//int i, j;

	LPCSTR portname = "COM7";                /*Ports will vary for each computer */
	DWORD dwEventMask = 0;                   /*Event mask that will be triggered */
	Port p(portname);
	bool startNewLog = false;

	/* SetCommMask sets the event that will cause a notification
	 * in this case, we use EV_RXCHAR which means that a new character was
	 * received a put in the input buffer
	 * https://docs.microsoft.com/en-us/previous-versions/ff802693(v=msdn.10)?redirectedfrom=MSDN */

	if (!SetCommMask(p.getHandle(), EV_RXCHAR))
	{
		perror("Error in setting CommMask ");
	}

	else
	{
		//cout << endl;
		//cout << "CommMask was successfully set." << endl;
	}

	//start an endless loop
	while (1)
	{
		/* Set WaitCommEvent */

        cout << "Waiting to receive data..." << endl;

        if (!WaitCommEvent(p.getHandle(), &dwEventMask, NULL))
		{
			perror("Error in setting WaitCommEvent ");
		}
		// we process the data
		else
		{
			p.receiveMessage();
		}

		if (!p.isBufferEmpty())
		{
			message = p.removeNextMessage();
			startNewLog = b.logFile(file, &message, 0);
			if (startNewLog)
            {
				b.createFileName(&fileName, 0);
				fileName = b.getPathToMessages() + fileName;
	            file.open(fileName, std::ios::out | std::ios::app);
            }
		}

		cout << "_________________________________________________________" << endl;

	} /* end while loop */

	/* close the serial port */
	p.closeSerialPort(p.getHandle());

	return 0;
} // end of program