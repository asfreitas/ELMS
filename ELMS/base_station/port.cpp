/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
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
returns the amount of data that was read
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
    HANDLE hSerial;
    std::string name;

    if (portname == NULL)
    {
        for (int x = 1; x <= 10; x++)
        {
            name = "COM" + std::to_string(x);
            portname = name.c_str();
            hSerial = createPort(portname);

            if (hSerial != INVALID_HANDLE_VALUE) 
            {
                return hSerial; // open the port if we find one that doesn't return an eror
            }
        }
    }
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

   return hSerial = CreateFile(portname, 
       accessdirection, 0, 0, OPEN_EXISTING ,0, 0);
}

/*
=============
Contructor - No Parameters

Opens a new port and attempts to find the port automatically 
=============
*/
Port::Port()
{
    openSerialPort(NULL);
    startPortThread();
    startTimer(5);

}

/*
=============
Contructor - Portname provided

Opens a new port and attempts to open the port passed into the constructor
=============
*/
Port::Port(LPCSTR portname)
{
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
    std::cout << "I am in the port destructor" << std::endl;
    stillReceiving = false;
    if (messageThread.joinable())
        messageThread.join();
    if (portThread.joinable())
        portThread.join();
   // waitCommMask(EV_TXEMPTY); // make sure that there is nothing left to send before closing
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
void Port::removeMessageFromBuffer(std::string *mystring)
{
    std::lock_guard<std::mutex> messageLock(mutex); // lock data for when it is being put into buffer

    std::string message = buffer.front();
    buffer.pop();
    *mystring = message;
}

/*
=============
receiveMessage
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
        if (networkFailure == true)
        {
            std::cout << "Network Failure\n";
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
startMessageThread
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
startMessageThread
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