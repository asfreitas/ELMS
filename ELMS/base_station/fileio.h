/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#include <fstream>
#include <istream>
#include <Windows.h>
#include <string>
#include <thread>
#include <mutex>
#include <vector>
#include <iostream>

using std::string;
using std::cout;
using std::endl;
#ifndef FILEIO_H
#define FILEIO_H

enum class MessageType { incoming, alert, network_failure, misc };

class FileIO
{
    private:
        // current directory locations. The pathToLogs is initialized in case
        // the user does not provide one to the constructor. 
        std::string pathToLogs;
        std::string pathToMessages;
        std::string pathToAlerts;
        std::string pathToNetworkFailure;
        std::string pathToMiscErrors;
        // current message counts need to be the same for all objects in the
        // class
        int messageCount = 1;
        int networkFailureCount = 1;
        int miscCount = 1;
        int alertCount = 1;
        int messageLimit;
        // filenames being written to need to be the same for all objects in
        // the class. 
        std::string logFileName;
        std::string alertFile;
        std::string netFailFile;
        std::string miscErrorFile;
        // remaining variables
        std::thread writing_thread;
        std::mutex mtx_write;

    public:
        FileIO(std::string pathToLogs, int messageLimit); // set the path to logs on creating the file handler
        FileIO();
        ~FileIO();
        bool checkMessageCount(MessageType);
        int& getMessageCount(MessageType);
        void logToFile(std::string inputMessage, MessageType);
        void logToExistingFile(std::string existingFile, string inputMessage, MessageType);
        void resetMessageCount(MessageType);
        std::string createFileName(MessageType);
        void setFileName(MessageType);
        void createFolders();
        bool createFolder(string);
        bool directoryExists(const std::string&);
        void lockWriteFile(string, string);
        void writeToFile(string, string);

        // get functions 
        int getMessageLimit();
        void getNewFilePath(MessageType);
        string getFilePath(MessageType);
        string getLogFilePath() { return logFileName; }
        string getalertFilePath() { return alertFile; }
        string getNetFailFilePath() { return netFailFile; }
        string getmiscErrorFilePath() { return miscErrorFile; }
        string getPathToMessages() { return pathToMessages; }
        string getPathToLogs() { return pathToLogs; }

};
#endif
