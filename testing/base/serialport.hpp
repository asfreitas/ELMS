#ifndef SERIALPORT_HPP
#define SERIALPORT_HPP
#include <windows.h>

/* This is from https://github.com/waynix/SPinGW 
 * The original code had enums to define baudrates, stopbits, and parity.
 * I removed them because Visual Studio required enum classes since
 * it is felt that enums can lead to "surprises" or bugs versus an
 * enum class. */ 

/**
    \brief Opens a new connection to a serial port
    \param portname		name of the serial port(COM1 - COM9 or \\\\.\\COM1-COM256)
    \param baudrate		the baudrate of this port (for example 9600)
    \param stopbits		th number of stopbits (one, onePointFive or two)
    \param parity		the parity (even, odd, off or mark)
    \return			HANDLE to the serial port
    */
HANDLE openSerialPort(LPCSTR portname, int baudrate, int stopbits, int parity);

/**
    \brief Read data from the serial port
    \param hSerial		File HANDLE to the serial port
    \param buffer		pointer to the area where the read data will be written
    \param buffersize	maximal size of the buffer area
    \return				amount of data that was read
    */
DWORD readFromSerialPort(HANDLE hSerial, char* buffer, int buffersize);
/**
    \brief write data to the serial port
    \param hSerial	File HANDLE to the serial port
    \param buffer	pointer to the area where the read data will be read
    \param length	amount of data to be read
    \return			amount of data that was written
    */
DWORD writeToSerialPort(HANDLE hSerial, char* data, int length);

void closeSerialPort(HANDLE hSerial);

#endif

