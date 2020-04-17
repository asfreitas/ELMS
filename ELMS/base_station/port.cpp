/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#include "port.h"


  /* Reference for this code and serialport.hpp is https://github.com/waynix/SPinGW */
  /********************************************************************************
   * Helpful reference to understand the Win32 API is:
   * https://www.xanthium.in/Serial-Port-Programming-using-Win32-API */



/*
=============
openSerialPort

portname contains name of serial port and stopbits may be either [1, 1.5, 2]
returns the handle of the port
=============

*/
void Port::openSerialPort(LPCSTR portname)
{

    hSerial = setupPort(portname); // get the port
    
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        // do something
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
DWORD Port::readFromSerialPort(HANDLE hSerial, char* buffer, int buffersize)
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
DWORD Port::writeToSerialPort(HANDLE hSerial, char* data, int length)
{
    DWORD dwBytesRead = 0;
    if (!WriteFile(hSerial, data, length, &dwBytesRead, NULL))
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
    setupPort(NULL);
}

/*
=============
Contructor - Portname provided

Opens a new port and attempts to open the port passed into the constructor
=============
*/
Port::Port(LPCSTR portname)
{
    setupPort(portname);
}

/*
=============
Destructor

Opens a new port and attempts to open the port passed into the constructor
=============
*/
Port::~Port()
{
    closeSerialPort(hSerial);
}