/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

/* This is from https://github.com/waynix/SPinGW
 * The original code had enums to define baudrates, stopbits, and parity.
 * I removed them because Visual Studio required enum classes since
 * it is felt that enums can lead to "surprises" or bugs versus an
 * enum class. */

#include <windows.h>
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
//#include "timer.h"
#include <vector>
using std::vector;
using std::string;

#ifndef SERIALPORT_HPP
#define SERIALPORT_HPP

class Port
{
    private:
        static const int messageSize = 50;
        HANDLE hSerial;
        std::queue<std::string> buffer;
        std::mutex mutex;
        std::thread portThread;
        std::thread messageThread;
        bool stillReceiving = true;
        bool networkFailure = false;
        bool portReady = false;
        //Timer t;
        

    public:
        Port(LPCSTR portname);
        Port();
        ~Port();
        void openSerialPort(LPCSTR); 
        DWORD readFromSerialPort(char*, int);
        DWORD writeToSerialPort(char*, int, HANDLE);
        void closeSerialPort(HANDLE);
        HANDLE setupPort(LPCSTR);
        HANDLE createPort(LPCSTR);
        HANDLE getHandle();
        void addToMessageBuffer(std::string);
        void removeMessageFromBuffer(std::string*);
        bool isBufferEmpty();
        void receiveMessage();
        void startPortThread();
        std::string getNextMessage();
        std::queue<std::string> getQueue() { return buffer; }
        void startTimer(int);
        void netFailureCheck(int);
        void setCommMask(DWORD);
        bool waitCommMask(DWORD);
        bool isPortReady() { return portReady; }
        // returns a list of possible COM ports available
        void SelectComPort(vector<string>&);
};

#endif
