/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#ifndef BASE_UNIT_H
#define BASE_UNIT_H


#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <Windows.h>
#include <vector>
using std::string;
using std::vector;

//this is how many messages will be in each log file before it is close. 
// this is easily changed and we can make any number we want. 
#define MESSAGE_LIMIT 5

class Base_Unit
{
    // using a static variable will be the same value for all class objects. 
    static int messageCount;
    static int alertCount;
    static int networkCount;
    static int miscCount;
    static string logFileName;
    static string alertFile;
    static string netFailFile;
    static string miscErrorFile;

    //this is the path to the folder that stores the logs. It has a set function
    // to allow the user to create the folder wherever they want.  
    string pathToLogs = "C:\\logs";
public:
    bool checkMessageCount(int type);
    bool logFile(std::ofstream &logFile, string* inputMessage, int type);
    void lockCloseFile(std::ofstream& logFile, int type);
    void lockWriteFile(std::ofstream& logFile, string* inputMessage);
    int getMessageCount(int type);
    void resetMessageCount(int type);
    void incMessageCount(int type);
    void createFileName(string* fileName, int type);
    void createFolder();
    bool directoryExists(const std::string& directoryName);
    void setPathToLogs(string& path);
    string getPathToLogs();
    string getPathToMessages();
    string getPathToAlerts();
    string getPathToNetWorkFailure();
    string getPathToMiscErrors();
    string getCurrentFileName(int type);
    void setFileName(int type);


};

#endif // !BASE_UNIT_H
