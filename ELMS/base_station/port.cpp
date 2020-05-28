/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the functions for port handling. 
*/
#include "port.h"


/* Reference for this code and serialport.hpp is https://github.com/waynix/SPinGW */
/********************************************************************************
 * Helpful reference to understand the Win32 API is:
 * https://www.xanthium.in/Serial-Port-Programming-using-Win32-API
 *
 */


 /************************************
 USAGE: In order to get messages from the buffer
 you must call the "getNextMessage" function
 *********************************/



 /*
 =============
 openSerialPort

 portname contains name of serial port and stopbits may be either [1, 1.5, 2]
 returns the handle of the port
 =============

 */
void Port::openSerialPort(LPCSTR portname)
{
    const int BYTE_SIZE = 8;
    const int READ_INTERVAL_TIMEOUT = 0; // TODO: figure out what values work best for the program
    const int READ_MULTIPLIER = 10;
    const int READ_CONSTANT = 50;
    const int WRITE_MULTIPLIER = 10;
    const int WRITE_CONSTANT = 50;
    const int baudrate = 9600;
    const int STOP_BITS = ONESTOPBIT;
    const int PARITY = NOPARITY;

    hSerial = setupPort(portname); // get the port

    if (hSerial == INVALID_HANDLE_VALUE)
    {
        perror("Error opening serial handle\n");
    }

    DCB dcbSerialParams = { 0 };
    dcbSerialParams.DCBlength = sizeof(dcbSerialParams);

    if (!GetCommState(hSerial, &dcbSerialParams))
    {
        perror("GetCommState ");
        DWORD dw = GetLastError();
        ExitProcess(dw);
    }
    dcbSerialParams.BaudRate = baudrate;
    dcbSerialParams.ByteSize = BYTE_SIZE;
    dcbSerialParams.StopBits = STOP_BITS;
    dcbSerialParams.Parity = PARITY;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        perror("SetCommState ");
        DWORD dw = GetLastError();
        ExitProcess(dw);
    }
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = READ_INTERVAL_TIMEOUT;
    timeouts.ReadTotalTimeoutConstant = READ_CONSTANT;
    timeouts.ReadTotalTimeoutMultiplier = READ_MULTIPLIER;
    timeouts.WriteTotalTimeoutConstant = WRITE_CONSTANT;
    timeouts.WriteTotalTimeoutMultiplier = WRITE_MULTIPLIER;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        perror("SetCommTimeouts ");
        DWORD dw = GetLastError();
        ExitProcess(dw);
    }
}

/*
=============
readFromSerialPort

hSerial		File HANDLE to the serial port

Reads the amount of data from the serial port 
and then returns the amount read.
=============
*/
DWORD Port::readFromSerialPort(char* buffer, int buffersize)
{
    DWORD dwBytesRead = 0;
    if (!ReadFile(hSerial, buffer, buffersize, &dwBytesRead, NULL))
    {
        perror("ReadFile ");
        DWORD dw = GetLastError();
        ExitProcess(dw);
    }
    return dwBytesRead;
}


/*
=============
readFromSerialPort

hSerial		File HANDLE to the serial port
length is for how much data is going to be written
returns the amount of data that was written

Reads a certain amount of data from the serial port and returns the amount
of data read
=============
*/
DWORD Port::writeToSerialPort(char* data, int length, HANDLE handle)
{
    DWORD dwBytesRead = 0;
    if (!WriteFile(handle, data, length, &dwBytesRead, NULL))
    {
        perror("WriteFile ");
        DWORD dw = GetLastError();
        ExitProcess(dw);
    }
    return dwBytesRead;
}

/*
=============
closeSerialPort

Closes the current serial port.
=============
*/
void Port::closeSerialPort(HANDLE hSerial)
{
    CloseHandle(hSerial);
}

/*
=============
detectHandle

Check for a handle until we find one that doesn't return an error
=============
*/
HANDLE Port::setupPort(LPCSTR portname)
{
    HANDLE hSerial = NULL;
    string temp;

    vector <string> listOfPorts;
    //declare a vector that will hold the list of ports available
    if (portname == NULL)
    {
        //if portname was NULL, then we call SelectComPort to get the list of
        // available ports. 
        SelectComPort(listOfPorts);

        //call getPort to get the available ports
        getPort(&listOfPorts, temp);

        //convert temp to LPCSTR
        portname = temp.c_str();
        //if the first letter of portname is 0, then this means portname 
        // returned "No Ports Detected"...so we exit. 
        if (portname[0] == 'N' || portname[0] == NULL)
        {
            cout << "No serial ports found or none were selected...exiting with code 1" << endl;
            exit(1);
        }
        else
            cout << "Waiting for data from mobile unit..." << endl;
        //we have a portname, so we call createPort
        hSerial = createPort(portname);
    }

    // The portnname is known, the the serial handle is created. 
    else
    {
        hSerial = createPort(portname); // if we know the port go ahead and open it
    }

    return hSerial;
}

/*
=============
testPort

Attempts to open a port with a particular portname
=============
*/
HANDLE Port::createPort(LPCSTR portname)
{
    HANDLE hSerial;
    DWORD  accessdirection = GENERIC_READ | GENERIC_WRITE;

    return hSerial = CreateFileA(portname,
        accessdirection, 0, 0, OPEN_EXISTING, 0, 0);
}

/*
=============
Contructor - No Parameters

Opens a new port and attempts to find the port automatically
=============
*/
Port::Port(FileIO* _f)
{
    openSerialPort(NULL);
    startPortThread();
    startTimer(5);

    fileHandler = _f;


}

/*
=============
Contructor - Portname provided

Opens a new port and attempts to open the port passed into the constructor
=============
*/
Port::Port(LPCSTR portname, FileIO* _f)
{
    fileHandler = _f;
    openSerialPort(portname);
    if (waitCommMask(EV_RXCHAR))
        portReady = true;
    startPortThread();
    startTimer(5);
}

/*
=============
Destructor

Shuts down the port and makes sure all data has been sent before closing
=============
*/
Port::~Port()
{
    stillReceiving = false;
    if (messageThread.joinable())
        messageThread.join();
    if (portThread.joinable())
        portThread.join();
    //waitCommMask(EV_TXEMPTY); // make sure that there is nothing left to send before closing
    closeSerialPort(hSerial);

}

/*
=============
getHandle
=============
*/
HANDLE Port::getHandle()
{
    return hSerial;
}

/*
=============
addToMessageBuffer
=============
*/
void Port::addToMessageBuffer(std::string message)
{
    std::lock_guard<std::mutex> receiveLock(mutex); // lock data for when it is being put into buffer

    buffer.push(message);
}

/*
=============
removeTopMessage

Removes the top message from the buffer and returns it
=============
*/
void Port::removeMessageFromBuffer(std::string* mystring)
{
    std::lock_guard<std::mutex> messageLock(mutex); // lock data for when it is being put into buffer

    std::string message = buffer.front();
    buffer.pop();
    *mystring = message;
}

/*
=============
receiveMessage

Gets new data from the port and puts them into the buffer.
=============
*/
void Port::receiveMessage()
{
    while (stillReceiving)
    {

        char newMessage[messageSize];
        memset(newMessage, 0, messageSize);
        char extra[2];

        readFromSerialPort(newMessage, messageSize - 15); // read in most of the message to ensure we don't bleed into next message
        std::string finalMessage(newMessage);
        if (finalMessage.size() > 1)
        {
            while (finalMessage[finalMessage.size() - 1] != '*') // finish combining the message
            {
                memset(extra, 0, 2);
                readFromSerialPort(extra, 1);
                finalMessage += std::string(extra); // to do: reduce the amount of casting necessary
            }
            addToMessageBuffer(finalMessage);

        }
        if (networkFailure)
        {
            std::string mystring;
            // *Reference: https://www.daniweb.com/programming/software-development/threads/476954/convert-from-localtime-to-localtime-s

            //get the current time
            time_t now = time(0);

            //declare a time structure
            struct tm gmtm;

            //use a thread safe call to get the current UTC time
            gmtime_s(&gmtm, &now);


            int hour = gmtm.tm_hour;
            int min = gmtm.tm_min;
            int sec = gmtm.tm_sec;

            mystring = "There was network failure from: " + std::to_string(hour) + " " + std::to_string(min) + " " + std::to_string(sec);
            std::cout << mystring << std::endl;
            auto start = std::chrono::system_clock::now();

            waitCommMask(EV_RXCHAR);

            waitCommMask(EV_RXCHAR);



            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end - start;
            //get the current time
            now = time(0);

            //use a thread safe call to get the current UTC time
            gmtime_s(&gmtm, &now);


            hour = gmtm.tm_hour;
            min = gmtm.tm_min;
            sec = gmtm.tm_sec;

            mystring += " to " + std::to_string(hour) + " " + std::to_string(min) + " " + std::to_string(sec) + " for " + std::to_string(diff.count()) + " seconds.\n";

            fileHandler->logToFile(mystring, MessageType::network_failure);


        }
    }

}

/*
=============
isBufferEmpty
=============
*/
bool Port::isBufferEmpty()
{
    return buffer.empty();
}

/*
=============
receiveDataFromSerialPort

Starts up the main thread that will continuously receive data from serial port
=============
*/
void Port::startPortThread()
{
    portThread = std::thread(&Port::receiveMessage, this);
}

/*
=============
getNextMessage

Waits to ensure that it gets the entire message without interrupting
buffer and returns next element in queue.
=============
*/
std::string Port::getNextMessage()
{
    std::string mystring;
    messageThread = std::thread(&Port::removeMessageFromBuffer, this, &mystring);
    messageThread.join();
    return mystring;
}

/*
=============
startTimer

Starts up the timer for checking network failure
=============
*/
void Port::startTimer(int numSeconds)
{
    std::thread netFailure = std::thread(&Port::netFailureCheck, this, numSeconds);
    netFailure.detach();
}

/*
=============
netFailureCheck

Check for network failure and set the networkFailure variable
=============
*/
void Port::netFailureCheck(int numSeconds)
{
    bool timesUp;
    bool currentNetworkFailure;
    while (stillReceiving)
    {
        auto start = std::chrono::system_clock::now();
        timesUp = false;
        currentNetworkFailure = true;
        while (!timesUp && currentNetworkFailure)
        {
            auto end = std::chrono::system_clock::now();
            std::chrono::duration<double> diff = end - start;

            if (diff.count() > numSeconds)
            {
                //std::this_thread::sleep_for(std::chrono::milliseconds(100));
                networkFailure = true;
                timesUp = true;
            }

            if (!isBufferEmpty())
            {
                networkFailure = false;
                currentNetworkFailure = false;
            }
        }

    }
}

/*
=============
setCommMask

// https://docs.microsoft.com/en-us/windows/win32/devio/monitoring-communications-events

Sets up a comm mask 
=============
*/
void Port::setCommMask(DWORD mask)
{
    bool setMask;
    setMask = SetCommMask(hSerial, mask);

    OVERLAPPED o;

    o.hEvent = CreateEvent(
        NULL,   // default security attributes 
        TRUE,   // manual-reset event 
        FALSE,  // not signaled 
        NULL    // no name
        );


    // Initialize the rest of the OVERLAPPED structure to zero.
    o.Internal = 0;
    o.InternalHigh = 0;
    o.Offset = 0;
    o.OffsetHigh = 0;

    if (!setMask)
    {
        // Handle the error. 
        printf("SetCommMask failed with error %d.\n", GetLastError());
        return;
    }
}

/*
=============
waitCommMask

Wait for a particular mask
=============
*/
bool Port::waitCommMask(DWORD mask)
{
    DWORD status, dwEventMask;
    setCommMask(mask);
    status = WaitCommEvent(hSerial, &dwEventMask, NULL);
    return dwEventMask;
}

/*
=============================================================================
SelectComPort

This function is from the following two references
 https://stackoverflow.com/questions/2674048/what-is-proper-way-to-detect-all-available-serial-ports-on-windows
 https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-querydosdevicew
  It will find the available COM ports and adds them to a vector list that is
  sent in by reference.

  ==========================================================================
  */
void Port::SelectComPort(vector <string>& comPortList) //added function to find the present serial 
{
    char lpTargetPath[5000]; // buffer to store the path of the COM PORTS
    bool isComPort = false; // represents if a COM port is available or not
    std::string str = "";
    for (int i = 0; i < 255; i++) // checking ports from COM0 to COM20
    {
        if (i > 9)
        {
            str += "\\\\.\\";
        }
        str += "COM" + std::to_string(i); // converting to COM0, COM1....

        // queries if there is a MS-DOS device.  Serial ports are this type
        // of device. 


        DWORD test = QueryDosDevice(str.c_str(), lpTargetPath, 5000);

        // Test the return value and error if any
        if (test != 0) //QueryDosDevice returns zero if it didn't find an object
        {
            //push the port onto the list of serial ports available.
            comPortList.push_back(str);
            isComPort = true;
        }

        if (::GetLastError() == ERROR_INSUFFICIENT_BUFFER)
        {
        }
    }
}