/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

/* This is from https://github.com/waynix/SPinGW
 * The original code had enums to define baudrates, stopbits, and parity.
 * I removed them because Visual Studio required enum classes since
 * it is felt that enums can lead to "surprises" or bugs versus an
 * enum class. */
#ifndef PORT_HPP
#define PORT_HPP
#include <windows.h>
#include <iostream>
#include <string>
#include <queue>


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
    static const int messageSize = 50;
    HANDLE hSerial;
    std::queue<std::string> buffer;

public:
    Port(LPCSTR portname);
    Port();
    ~Port();
    void openSerialPort(LPCSTR);
    DWORD readFromSerialPort(char*, int);
    DWORD writeToSerialPort(char*, int);
    void closeSerialPort(HANDLE);
    HANDLE setupPort(LPCSTR);
    HANDLE createPort(LPCSTR);
    HANDLE getHandle();
    void addToMessageBuffer(std::string);
    std::string removeNextMessage();
    bool isBufferEmpty();
    void receiveMessage();

};
#endif
