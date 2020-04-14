
/*
References:
 * https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
 * https://github.com/xanthium-enterpreises/Serial-Programming-Win32API-C
 * https://docs.microsoft.com/en-us/cpp/c-runtime-library/reference/strtok-s-strtok-s-l-wcstok-s-wcstok-s-l-mbstok-s-mbstok-s-l?view=vs-2019 

*/

#include <Windows.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include "serialport.hpp"
#include <vector>
#include "vehicle.hpp"
#include "vector_utilities.hpp"
#include <iostream>
#include <sstream> /* allows use of ostringstream */
using std::cout;
using std::endl;


/* define constants used for serial port functions*/
#define BAUDRATE 9600
#define STOPBITS ONESTOPBIT
#define PARITY NOPARITY


/* reference for the following -- this was an issue if you are using Visual Studio
 * https://stackoverflow.com/questions/22210546/whats-the-difference-between-strtok-and-strtok-r-in-c/22210711       */

#ifdef _MSC_VER
#define strtok_r strtok_s
#endif

/* A note about how to compile this program using a makefile in mingw
 * change directory to your files. Then run: mingw32-make all
 * to clean: mingw32-make clean */


int main(int argc, char *argv[])
{
    //declare iterators that will be used
    int i, j;	

    /* declare the variables that will store the data*/
    int unit_id, time, velocity, bearing;
	
	/* declared latitude and longitude as doubles because to do distance
	// calculations, we will need to convert them to degrees decimal, then to
	// radians. We need to consider if we will use Haversine to calculate 
	// the distance between two points given the latitude and longitude 
	 * reference: https://community.esri.com/groups/coordinate-reference-systems/blog/2017/10/05/haversine-formula
	 * Dr. Kilty said that this gives much more precision when when only need within 2-3.
     * He brought this up because he was concerned that Haversine might "slow" the program
     * with the calculations. He said our level of accuracy for now needs to be 2 or 3 meters. 	 
	 * He said we may wish to consider a method called transverse mercator.(need to research this)
     * Both involve calling trig functions so I am not sure one would take more time than another. */ 
    double latitude, longitude;
	
    HANDLE hSerial;                          /*Handle to the Serial port */
    LPCSTR portname = argv[1];                /*Ports will vary for each computer */
    DWORD dwEventMask = 0;                   /*Event mask that will be triggered */
    char  tempChar;                          /*Temporary char used to read the data */
    char  serialBuffer[1024];                /*Buffer Containing Received Data */
    DWORD numBytesToRead;                    /*Bytes read by ReadFile() */
	
	/* This is a character array that holds the notification to send back
	 * to the mobile unit.*/
    char notification[50];
    
	/* These tokens are used for the parsing process. */
    char* next_token = NULL;
	char* token = NULL;
	
	/*The delimiters.  */
	const char delimiters[] = "$ * ,";
	
	/* Vector that holds the Vehicle objects. */
	vector<Vehicle>mine_vehicle;
	
	/*open the serial port */
    hSerial = openSerialPort(portname, BAUDRATE, STOPBITS, PARITY);
	
	/* SetCommMask sets the event that will cause a notification
     * in this case, we use EV_RXCHAR which means that a new character was
     * received a put in the input buffer
     * https://docs.microsoft.com/en-us/previous-versions/ff802693(v=msdn.10)?redirectedfrom=MSDN */

    if (!SetCommMask(hSerial, EV_RXCHAR))
    {
        perror("Error in setting CommMask ");
    }

    else
    {
		cout << endl;
        cout << "CommMask was successfully set." << endl;
    }


    while (1)
    {
        i = 0;
		
		/*re-initialize the buffer */
        memset(serialBuffer, '\0', 1024);
		
        /* Set WaitCommEvent */
		cout << endl;
		cout << "Waiting to receive data..." << endl;

        if (!WaitCommEvent(hSerial, &dwEventMask, NULL))
        {
            perror("Error in setting WaitCommEvent ");
        }

        else /* the WaitCommEvent has received data so we will use ReadFile to
              * get the data and load it into the serialBuffer */
        {
            do
            {
				/* we are reading the input char by char and storing it in the buffer */
                if (ReadFile(hSerial, &tempChar, sizeof(tempChar), &numBytesToRead, NULL))
                {

                    serialBuffer[i] = tempChar;
					/* i is being used to count the number of chars received */
                    i++;
                }

            } while (numBytesToRead > 0);

            /* print the received data to the terminal */
			cout <<endl;
            cout<<"Data received: ";
            for (j = 0; j < i - 1; j++)
            {	
                cout <<serialBuffer[j];				
            }
			cout << endl << endl;
			
			//This for-loop is only meant to find the start of data which is delineated by
			// by '$'. It removes '$' from the start of the C-String, then begins the
			// parsing process. There are probably much more efficient ways to accomplish
			// the parsing. I found that since '$' is at the start of the sentence with 
			// nothing in front of it, strtok was skipping over the initial data
			for (j = 0; j < i-1; j++)
            {				
			
				if(serialBuffer[j] == '$')
				{
					/*https://stackoverflow.com/questions/4295754/how-to-remove-first-character-from-c-string */
					memmove(serialBuffer, serialBuffer+1, strlen(serialBuffer));
					break;
			    }				
            }
            next_token = serialBuffer;
			
			while ((token = strtok_r(next_token, delimiters, &next_token)))
			{
				j++;
				if(j == 1)
				{					
					/* convert the token to an int */
					unit_id = atoi(token);
				}
				else if (j == 2)
			    {
					/* change time to an int */
					time = atoi(token);
					
			    }
				else if (j == 3)
				{
					/* convert latitude to a double */
					latitude = atof(token);
				}
				else if (j == 4)
				{
					/* convert longitude to a double */
					longitude = atof(token);
				}
				else if ( j == 5)
				{
					/* convert velocity to an int */
					velocity = atoi(token);
				}
				else if (j == 6)
				{
					/* convert bearing to an int */
					bearing = atoi(token);
				}
				else
				{
					/* Do nothing */
				}
				
			}
			
			/* This part of the code is only meant to demo how to use the serial port to write
			 * and send a message to the mobile unit. I selected unit 20 by times. 
			 * Since we have not done any calculations, it is just sample data but it
			 * will demonstrate how the information is transmitted to the mobile unit */
			if(unit_id == 20 && (time > 100099 && time < 100241))
			{
					/* Per specifications, message format 
					 * $unit x-> unit y, time stamp, speed, distance, bearing*
					 * Unit x and unit y are numeric for a piece of mine equipment
					 * Time is 6 digit in Zulu time
					 * Speed in meters per second
					 * Distance is in meters and is value from x to y
					 * Bearing is x relative to y
					 * ASCII, comma separated, starts with $ and ends with *
					 * x is the originating unit number (in message above) and y is the unit number being warned about presence of x*/

                    /* re-initialize the notification char array */
					memset(notification, '\0', 50);
				
				
					/*Two different ways to get the char array loaded with the data.
                     * The first way uses cpp libraries.  The second way uses sprintf 
					 *  which visual studio did not like */
					int b;
					int distance = 50;
					
					/* comment out the code starting from here */
					int length;
					std::ostringstream out;
					out << "$" << unit_id << " -> " << 3 << "," << time << "," << velocity << "," << distance << "," << bearing << "*";
					std::string info = out.str();
					length = info.length();
					info.copy(notification, length);
					
					notification[length] = '\r';
					notification[length + 1] = '\0';
					
					b=strlen(notification);
					writeToSerialPort(hSerial, notification, b);
					/* to here if you want to use ostringstream to "build" a string" */

                    
					/* uncomment the code from here */
					
					/*sprintf(notification, "$ %d -> %d,%d,%d,%d,%d * \n",unit_id,3,time,50, velocity,bearing);
					strcat(notification,"\r\0");
					b=strlen(notification);
					writeToSerialPort(hSerial, notification, b);
					*/
					
					/* to here to use sprintf method */
					
			} 
            /* creates Vehicle class object and pushes into vector if not duplicate */    
            input_data(mine_vehicle, unit_id, time, latitude, longitude, velocity, bearing);
			
			/* prints what is currently in the vector */
			print_vector(mine_vehicle);			
        }
        cout << endl;
		cout <<"_________________________________________________________" << endl;
		
    } /* end while loop */
	
	/* close the serial port */
	CloseHandle(hSerial);
    
    return 0;
} // end of program