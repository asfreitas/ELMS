/*
ELMS Project
This program tracks the location of vehicles by receiving GPS coordinates through
a radio transceiver. It stores the locations and determines the distance the
vehicle is from other vehicles or equipment. The vehicle receives a priority
number on a 0 - 3 scale.
0 = The vehicle is within 50 or less meters from another vehicle or piece of equipment
1 = The vehicle is 51 - 75 meters away from another vehicle or piece of equipment
2 = The vehicle is 76 - 100 meters away from another vehicle or piece of equipment
3 - The vehicle is 101 or greater meters away from another vehicle or piece of equipment

If a vehicle is priority 0, then an alert message is sent to the mobile units
involved. 

When the program starts, a directory called logs is created, if it does not exit, 
with subdirectories (1) alerts, (2) incoming_messages, (3) misc_errors, (4) 
network_failure.  Each incoming message is stored in the incoming_messages folder,
alerts are stored in the alerts folder, and possible network_failure in the
network_failure folder. 

The data is also stored and in a database that allows for analytics.. 

References:
 * https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
 * https://github.com/xanthium-enterpreises/Serial-Programming-Win32API-C
 * Reference used to get the current time and date to name the files
 * https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
*/

#include <omp.h>
#include "parse_incoming.h"
#include "base_unit.h"
#include "port.h"
#include <conio.h>
#include "base_unit_gui.h"

/*
 * reference for how to look for memory leaks in windows.
 * Youtube video. https://www.youtube.com/watch?v=t1wqj6J6Vhs
 */
#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
using std::cout;
using std::endl;
using std::ofstream;
using std::ios;
using std::string;
using std::thread;


 /* To use OpenMP if you are using Visual Studio you may need to change your
  * configuration Properties-> C/C++ -> Language -> Language -> OpenMP Support
  * to "Yes(/openmp)".   Reference for this is CS 475 Parallel Programming
  * Notes OpenMP Lecture Slide 6 written by Professor Bailey from Oregon
  * State University */

#ifndef _OPENMP
void printf_notice();
#endif


int main()
{
	//used to check for memory leak. When the program exists, it will dump all
	// any memory leaks that are present.  You must run in debug to see them. 
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);

	// declare a FILEIO and Base_Unit object
	FileIO f;
	Base_Unit b;

	string fileName;
	string incomingMessage;
	string data;
	unsigned long int index;

	 //declare the vector that will contain all of the vehicles in the mine
	vector<Vehicle*>mineVehicles;

	LPCSTR portname = NULL;//"COM3";                /*Ports will vary for each computer */
	Port p(portname, &f);

	//declare a pointer to a Vehicle v
	Vehicle* vehicle;

	//start an endless loop
	while (p.isPortReady())
	{
        //_kbhit returns a non-zero value if a key stroke was made
		if (_kbhit())
		{
			//define a char to capture the ASCII for the keystroke entered
			//if is important to keep this line in or else the pop-up asking
			// the user if they wish to quit will keep appearing. Cases were
			// not used here because the program will accept any keystroke
			char command = _getch();
			BOOL result = closeProgram1();
			if (result)
				return 0;				
		}

		if (!p.isBufferEmpty())
		{
			//get the next incoming message
			incomingMessage = p.getNextMessage();

			//make a copy of message that we will use to parse
			data = incomingMessage;

			// this variable checks to see if the incoming message is properly separated
	        // by commas.  
			size_t number = countCommas(incomingMessage);

			// This line of code declares two threads that will be used in 
			// the parallel section
			omp_set_num_threads(2);

            #pragma omp parallel sections
			{
                #pragma omp section
				{
					cout << incomingMessage << endl;
					
					b.logToFile(incomingMessage, MessageType::incoming);
				}

                #pragma omp section
				{
					//if the number of commas is 6, then the message is correctly
					// formed.
					if (number == 5)
					{
						// declare a pointer to a message struct
						struct message* ptr = createNewMessage(data);

						//declare a pointer to a vehicle object that will either be
						// added to the vector of mine vehicles
						vehicle = new Vehicle();

						//set the index to the size of the mineVehicles
						index = static_cast<int>(b.getMineVehicles().size());

						// add the vehicle pointer to the vector
						b.addToMineVehicles(vehicle);

						//this function is going to have a mutex and lock within the input_data function
						HANDLE h = p.getHandle();

						//input the new data
						b.input_data(index, ptr, p, h);

						// free the ptr memory
						delete ptr;
					}
					else
					{
						cout << "There was an error in the incoming message" << endl;
					}

				}
			}
			// only print again if we added or updated vehicles
			//b.print_vector(b.getMineVehicles());
		}
		else
		{
			//cout << "The buffer is empty" << endl;
		}

	} /* end while loop */

	/* No need to close the serial port because the class destructor automatically
	 * does this */
    return 0;
}

/* This void function will print a notice to the terminal if openMP is not
   supported */
void printf_notice()
{
	fprintf(stderr, "OpenMP is not supported \n");
}
