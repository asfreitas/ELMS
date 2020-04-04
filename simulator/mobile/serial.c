#include <stdio.h>
#include <windows.h>
#include "serialport.h"

/* To compile this project and not get a warning that about incompatible type
 * of parameter LPCWSTR go to Project-> Properties -> Advanced -> Character ->
 * Use MutiByte Character Set.  Or you can cast all the constant character strings
 * with (LPCWSTR).
 * Reference:
  *https://stackoverflow.com/questions/33001284/incompatible-with-parameter-of-type-lpcwstr
  */


/**
    \brief Opens a new connection to a serial port
    \param portname		name of the serial port(COM1 - COM9 or \\\\.\\COM1-COM256)
    \param baudrate		the baudrate of this port (for example 9600)
    \param stopbits		th nuber of stoppbits (one, onePointFive or two)
    \param parity		the parity (even, odd, off or mark)
    \return			HANDLE to the serial port
    */
HANDLE openSerialPort(LPCSTR portname, enum Baudrate baudrate, enum Stopbits stopbits, enum Paritycheck parity)
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
    dcbSerialParams.ByteSize = 8;
    dcbSerialParams.StopBits = stopbits;
    dcbSerialParams.Parity = parity;
    if (!SetCommState(hSerial, &dcbSerialParams)) {
        perror("SetCommState ");
        DWORD dw = GetLastError();
        ExitProcess(dw);
    }
    COMMTIMEOUTS timeouts = { 0 };
    timeouts.ReadIntervalTimeout = 50;
    timeouts.ReadTotalTimeoutConstant = 50;
    timeouts.ReadTotalTimeoutMultiplier = 10;
    timeouts.WriteTotalTimeoutConstant = 50;
    timeouts.WriteTotalTimeoutMultiplier = 10;
    if (!SetCommTimeouts(hSerial, &timeouts)) {
        perror("SetCommTimeouts ");
        DWORD dw = GetLastError();
        ExitProcess(dw);
    }
    return hSerial;
}

/**
    \brief Read data from the serial port
    \param hSerial		File HANDLE to the serial port
    \param buffer		pointer to the area where the read data will be written
    \param buffersize	maximal size of the buffer area
    \return				amount of data that was read
    */
DWORD readFromSerialPort(HANDLE hSerial, char* buffer, int buffersize)
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

/**
    \brief write data to the serial port
    \param hSerial	File HANDLE to the serial port
    \param buffer	pointer to the area where the read data will be read
    \param length	amount of data to be read
    \return			amount of data that was written
    */
DWORD writeToSerialPort(HANDLE hSerial, char* data, int length)
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

void closeSerialPort(HANDLE hSerial)
{
    CloseHandle(hSerial);
}
