/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#include <iostream>
#include <fstream>
#include <istream>
class Vehicle
{
    // using a static variable will be the same value for all class objects. 
    static int messageCount;
    static bool fileOpen;
    bool checkMessageCount();
    void logFile(std::fstream  *logFile, char * inputMessage);
    void lockCloseFile(std::fstream *logFile);
    void lockWriteFile(std::fstream* logFile, char* inputMessage);
    int getMessageCount();
    void resetMessageCount();
    bool getFileOpen();
    void setFileOpen();


};

int Vehicle::messageCount = 0;
bool Vehicle::fileOpen = false;