/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

/* This is from https://github.com/waynix/SPinGW
 * The original code had enums to define baudrates, stopbits, and parity.
 * I removed them because Visual Studio required enum classes since
 * it is felt that enums can lead to "surprises" or bugs versus an
 * enum class. */
#ifndef SERIALPORT_HPP
#define SERIALPORT_HPP
#include <windows.h>
#include <iostream>
#include <string>


class Port
{
    private:
        const int BYTE_SIZE = 8;
        const int READ_INTERVAL_TIMEOUT = 0;
        const int READ_MULTIPLIER = 10;
        const int READ_CONSTANT = 50;
        const int WRITE_MULTIPLIER = 10;
        const int WRITE_CONSTANT = 50;
        const int baudrate = 9600;
        const int STOP_BITS = ONESTOPBIT;
        const int PARITY = NOPARITY;
        HANDLE hSerial;

    public:
        Port(LPCSTR portname);
        Port();
        ~Port();
        void openSerialPort(LPCSTR portname); 
        DWORD readFromSerialPort(HANDLE hSerial, char* buffer, int buffersize);
        DWORD writeToSerialPort(HANDLE hSerial, char* data, int length);
        void closeSerialPort(HANDLE hSerial);
        HANDLE setupPort(LPCSTR portname);
        HANDLE createPort(LPCSTR portname);

};
#endif
