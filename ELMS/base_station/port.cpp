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
HANDLE Port::openSerialPort(LPCSTR portname, int baudrate, int stopbits, int parity)
{
    DWORD  accessdirection = GENERIC_READ | GENERIC_WRITE;
    HANDLE hSerial = CreateFile(portname,
        accessdirection,
        0,
        0,
        OPEN_EXISTING,
        0,
        0);
    if (hSerial == INVALID_HANDLE_VALUE)
    {
        perror("CreateFile ");
        DWORD dw = GetLastError();
        ExitProcess(dw);
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
    dcbSerialParams.StopBits = stopbits;
    dcbSerialParams.Parity = parity;
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
    return hSerial;
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
=============
*/
HANDLE Port::detectHandle()
{
    DWORD  accessdirection = GENERIC_READ | GENERIC_WRITE;
    HANDLE handle = CreateFile(portname,
        accessdirection,
        0,
        0,
        OPEN_EXISTING,
        0,
        0);
    return handle;
}