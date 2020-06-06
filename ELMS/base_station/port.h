/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*
* This file contains the headers for port handling.
*/

// This is from https://github.com/waynix/SPinGW

#ifndef SERIALPORT_HPP
#define SERIALPORT_HPP

#include "fileio.h"
#include "base_unit_gui.h"
#include "utilities.h"

#include <windows.h>
#include <iostream>
#include <string>
#include <queue>
#include <thread>
#include <mutex>
#include <chrono>
#include <vector>
#include <time.h>
#include <ctime>

using namespace std::chrono;

using std::vector;
using std::string;


static vector<string> listOfPorts;

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
    FileIO* fileHandler;
    bool closing = false;


public:
    Port(LPCSTR portname, FileIO*);
    Port(FileIO*);
    Port() {};
    ~Port();
    void openSerialPort(LPCSTR);
    bool readFromSerialPort(char*, int);
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
    bool getNetworkFailure() { return networkFailure; }
    bool isPortReady() { return portReady; }
    // returns a list of possible COM ports available
    void SelectComPort(vector<string>&);
    void setClosing(bool _closing) { closing = _closing; }
    void handleNetworkFailure();
    //void signalHandler(int signum);


};

#endif