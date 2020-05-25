/*
References:
 * https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
 * https://github.com/xanthium-enterpreises/Serial-Programming-Win32API-C
 * https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtok-s-strtok-s-l-wcstok-s-wcstok-s-l-mbstok-s-mbstok-s-l?view=vs-2019
 * Reference used to get the current time and date to name the files
 * https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
*/

#include <omp.h>
#include "parse_incoming.h"
#include "base_unit.h"
#include "port.h"


#define _CRTDBG_MAP_ALLOC
#include <crtdbg.h>
using std::cout;
using std::endl;
using std::ofstream;
using std::ios;
using std::string;
using std::thread;


/* reference for the following -- this was an issue if you are using Visual Studio
 * https://stackoverflow.com/questions/22210546/whats-the-difference-between-strtok-and-strtok-r-in-c/22210711
 * reference for how to look for memory leaks in windows.
 * Youtube video. https://www.youtube.com/watch?v=t1wqj6J6Vhs
 */

 /* To use OpenMP if you are using Visual Studio you may need to change your
  * configuration Properties-> C/C++ -> Language -> Language -> OpenMP Support
  * to "Yes(/openmp)".   Reference for this is CS 475 Parallel Programming
  * Notes OpenMP Lecture Slide 6 written by Professor Bailey */

#ifndef _OPENMP
void printf_notice();
#endif


int main()
{

	//used to check for memory leak. When the program exists, it will dump all
	// any memory leaks that are present.  You must run in debug to see them. 
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// declare a base class object
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

	bool startNewLog = false;

	//declare a pointer to a Vehicle v
	Vehicle* vehicle;

    // this counter is only here for testing purposes.
	int count = 0;

	//start an endless loop
	while (p.isPortReady() && count < 25)
	{

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
			count++;
			// only print again if we added or updated vehicles
			b.print_vector(b.getMineVehicles());
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
