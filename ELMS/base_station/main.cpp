/*
References:
 * https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
 * https://github.com/xanthium-enterpreises/Serial-Programming-Win32API-C
 * https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtok-s-strtok-s-l-wcstok-s-wcstok-s-l-mbstok-s-mbstok-s-l?view=vs-2019
 * Reference used to get the current time and date to name the files
 * https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
*/

#include <Windows.h>
#include <mutex>
#include <thread>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "port.h"
#include <vector>
#include "base_unit.h"
#include <iostream>
#include <fstream>
# include <string>
#include <ctime>
#include <time.h>
#include <omp.h>
#include "parse_incoming.h"
#include "base_unit_gui.h"
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

//int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrevInst, LPSTR args, int ncmdshow)
int main()
{
	//getPort();
	//startWindow();
	FileIO file("C:\\logs", 5);

	//used to check for memory leak. When the program exists, it will dump all
	// any memory leaks that are present.  You must run in debug to see them. 
	_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	// declare a base class object

	Base_Unit b;

	string fileName;
	string incomingMessage;
	string data;
	//string path = b.getPathToLogs();
	int index;

	 //declare the vector that will contain all of the vehicles in the mine
	vector<Vehicle*>mineVehicles;

	//This function is called to create the log directories that will be 
	// used in the program...if they do not already exist. 
	//b.createFolder();
	file.createFolders();

	// this function call creates the name of the file and the path to it. 
	// this will be used to open the files and write to them. 
	// add what type of file it is.  0 = incoming message, 1 = alert, 2 = network failure
	// 3 = misc errors
	//b.getFilePath(fileName, 0)
	LPCSTR portname = "COM7";                /*Ports will vary for each computer */
	Port p(portname);

	bool startNewLog = false;

	//declare a pointer to a Vehicle v
	Vehicle* vehicle;

    // this counter is only here for testing purposes.
	int count = 0;
	//start an endless loop
	while (p.isPortReady() && count < 2)
	{

		if (!p.isBufferEmpty())
		{
			//cout << "Buffer is not empty" << endl;
			incomingMessage = p.getNextMessage();
			//make a copy of message that we will use to parse
			data = incomingMessage;

			// This line of code declares two threads that will be used in 
			// the parallel section
			omp_set_num_threads(2);

            #pragma omp parallel sections
			{
                #pragma omp section
				{
					cout << incomingMessage << endl;
					file.logToFile(incomingMessage, MessageType::incoming);
					//b.logFile(fileName, &incomingMessage, 0);
				}

                #pragma omp section
				{
					// declare a pointer to a message struct
					struct message* ptr = createNewMessage(data);
					//declare a pointer to a vehicle object that will either be
					// added to the vector of mine vehicles
					vehicle = new Vehicle();
					//set the index to the size of the mineVehicles
					index = b.getMineVehicles().size();
					// add the vehicle pointer to the vector
					b.addToMineVehicles(vehicle);
					//this function is going to have a mutex and lock within the input_data function
					HANDLE h = p.getHandle();
					b.input_data(index, ptr, p, h);
					// free the ptr memory
					delete ptr;

				}
			}
			count++;
			// only print again if we added or updated vehicles
			//mineVehicles = b.getMineVehicles();
			b.print_vector(b.getMineVehicles());
		}
		else
		{
			//cout << "The buffer is empty" << endl;
		}

		std::this_thread::sleep_for(std::chrono::seconds(1));
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
