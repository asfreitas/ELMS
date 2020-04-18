#include "base_unit.h"
#include <iostream>
#include <stdbool.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
#include <ctime>
#include <cstdio>
#include <cstring>

using std::thread;
using std::cout;
using std::endl;

// declare a mutex that locks when the program is writing to a log file
std::mutex mtx_close;
std::mutex mtx_write;

/* Static members must be initialized outside of the class
 * Since it is a static member, it must be initialized outside of the class
 */

int Base_Unit::messageCount = 1;
int Base_Unit::alertCount = 1;
int Base_Unit::networkCount = 1;
int Base_Unit::miscCount = 1;
string Base_Unit::logFileName = "";
string Base_Unit::alertFile = "";
string Base_Unit::netFailFile = "";
string Base_Unit::miscErrorFile = "";


  /*
   * Function: checkMessageCount(int type)
   * This function takes the message and checks to see if the messages have
   * reached their limit. 
   * types are: 
   * type = 0 means that it is an incoming message
   * type = 1 means that it is an alert being sent to a vehicle
   * type = 2 means that it is a network failure message
   * type = 3 means that it is a miscellaneous error.
   * If message count is < MESSAGE_LIMIT, increment the message count and return false.
   * Otherwise, true is returned.
   */
bool Base_Unit::checkMessageCount(int type)
{
    if (getMessageCount(type) < MESSAGE_LIMIT)
    {
        incMessageCount(type);
        return false;
    }
    else
    {
        return true;

    }

}

/*
 * Function logFile
 * This function checks the message count. If it is < MESSAGE_LIMIT, then the inputMessage
 * is written to a log file and the message count is incremented. If the
 * message count is >= MESSAGE_LIMIT, then this last message is written to the log file
 * and the log file is closed.  This log file will be stored in the log file
 * directory and at regular intervals uploaded to the database.  When it is
 * closed it will be saved with a name created by using the local time. (we
 * may need to change this is Zulu time later)
 * References: https://www.cplusplus.com/reference/mutex/mutex/lock/
 * https://cplusplus.com/forum/general/194132

 * type = 0 means that it is an incoming message
 * type = 1 means that it is an alert being sent to a vehicle
 * type = 2 means that it is a network failure message
 * type = 3 means that it is a miscellaneous error.
*/

bool Base_Unit::logFile(std::ofstream& logFile, string* inputMessage, int type)
{
    bool needNewFile = false;

    // flag that will be true when both writing and closing to a file have finished. 
    bool done = false;

    //check to see if the number of messages written to the file is at the limit
    // It checks for the message type to determine if more should be written to
    // that particular log file. 
    bool messageCountAtLimit = checkMessageCount(type);

    //this loop while continue until both the writing and the closing of the 
    // log file have finished.  It prevents another message trying to write to
    // a file that is being closed. 
    while (messageCountAtLimit && done == false)
    {
        //declare a thread that will write this last message to the log file
        thread t1 = std::thread(&Base_Unit::lockWriteFile, this, std::ref(logFile), inputMessage);

        // have the thread join
        t1.join();
        // declare a thread and call the function to lock writing to the log file
        thread t2 = std::thread(&Base_Unit::lockCloseFile, this,  std::ref(logFile), type);

        //have the thread join again
        t2.join();
        done = true;

        //set the bool to true that a new file is needed
        needNewFile = true;
    }

    //messageCount was less than MESSAGE_LIMIT and the count was incremented. 
    // Call the function that writes the data to the current log file.
    if (!messageCountAtLimit)
    {
        bool d = false;
        while (!d)
        {
            thread t1 = std::thread(&Base_Unit::lockWriteFile, this, std::ref(logFile), inputMessage);

            // have the thread join again
            t1.join();
            d = true;
        }
        
    }
    //return the bool that represents if a new file is needed or not. 
    return needNewFile;
}

/* Function lockCloseFile
 * This function locks while closing a file so that it can be stored in its folder
 * type = 0 means that it is an incoming message
 * type = 1 means that it is an alert being sent to a vehicle
 * type = 2 means that it is a network failure message
 * type = 3 means that it is a miscellaneous error.
 * Reference: https://www.daniweb.com/programming/software-development/threads/476954/convert-from-localtime-to-localtime-s
 */
void Base_Unit::lockCloseFile(std::ofstream& logFile, int type)
{
    std::lock_guard<std::mutex> lck{ mtx_close };
    logFile.close();

    // reset the messageCounter for the particular type to 1;
    resetMessageCount(type);

    //don't need to unlock because using lock_guard

    return;
}

/* Function lockWriteFile locks any other part of the program writing to
 * log files while another process is writing to it. */
void Base_Unit::lockWriteFile(std::ofstream& logFile, string* inputMessage)
{
    //lock writing to a log file
    mtx_write.lock();

    // if the log file is open, then write to it. 
    if (logFile.is_open())
    {
        logFile << *inputMessage;
    }
 
    //open the lock
    mtx_write.unlock();
    
    return;

}

/*Function is a getter for messageCount
 * type = 0 means that it is an incoming message
 * type = 1 means that it is an alert being sent to a vehicle
 * type = 2 means that it is a network failure message
 * type = 3 means that it is a miscellaneous error.
*/
int Base_Unit::getMessageCount(int type)
{
    if (type == 0)
    {
        cout << "The message count is: " << messageCount << endl;
        return messageCount;
    }
    else if (type == 1)
    {
        return alertCount;
    }
    else if (type == 2)
    {
        return networkCount;
    }
    else
    {
        return miscCount;
    }
}

/*This function increments the current messageCount by 1 depending on the type
 * type = 0 means that it is an incoming message
 * type = 1 means that it is an alert being sent to a vehicle
 * type = 2 means that it is a network failure message
 * type = 3 means that it is a miscellaneous error.
*/
void Base_Unit::incMessageCount(int type)
{
    if (type == 0)
    {
        messageCount++;
    }
    else if (type == 1)
    {
        alertCount++;
    }
    else if (type == 2)
    {
        networkCount++;
    }
    else
    {
        miscCount++;
    }
}

/* Function creates a file name based on the time by year, month, day, hour, 
 * min, and second.  It also adds the type of message that it is. 
 * type = 0 = I means that it is an incoming message
 * type = 1 = A means that it is an alert being sent to a vehicle
 * type = 2 = N means that it is a network failure message
 * type = 3 = M means that it is a miscellaneous error. 
 * .txt is also appended as the extention for the files. 
 * Reference: https://stackoverflow.com/questions/997946/how-to-get-current-time-and-date-in-c
*/
void Base_Unit::createFileName(string *fileName, int type)
{
    string type_of_message;
    if (type == 0)
        type_of_message = "I";
    else if (type == 1)
        type_of_message = "A";
    else if (type == 2)
        type_of_message = "N";
    else
        type_of_message = "M";

    time_t current_time;
    struct tm local_time;
    time(&current_time);
    localtime_s(&local_time, &current_time);
    int year = local_time.tm_year + 1900;
    int month = local_time.tm_mon + 1;
    int day = local_time.tm_mday;
    int hour = local_time.tm_hour;
    int min = local_time.tm_hour;
    int sec = local_time.tm_sec;

    //build the string needed for the name of the file. 
    string tempName = std::to_string(year) + '_' + std::to_string(month) + '_' + std::to_string(day) + '_' +
        std::to_string(hour) + '_' + std::to_string(min) + '_' + std::to_string(sec) + '_' + type_of_message;

    //add to the end of the string the file extension type. 
    tempName = tempName + ".txt";

    //set the input string to the temporary string name created using the date/time
    *fileName = tempName;

}

/* Function resets messageCount to 1 when a new log file is opened depending on message type
 * type = 0 = I means that it is an incoming message
 * type = 1 = A means that it is an alert being sent to a vehicle
 * type = 2 = N means that it is a network failure message
 * type = 3 = M means that it is a miscellaneous error. */
void Base_Unit::resetMessageCount(int type)
{
    if (type == 0)
    {
        messageCount = 1;
    }
    if (type == 1)
    {
        alertCount = 1;
    }
    if (type == 2)
    {
        networkCount = 1;
    }
    else
    {
        miscCount = 1;
    }
}

/*This function first checks to see if a directory exists, then it creates the
 * logs directory and the 4 subfolders (1) incoming_messages (2) alerts (3) network_failure
 * (4) misc_errors. It will create the main logs file in whatever  */
// https://www.youtube.com/watch?v=B999K9yztnI
void Base_Unit::createFolder(string& pathToCreate)
{
    bool exists = false;
    exists = directoryExists(pathToCreate);
    int bDir;
    if (!exists)
    {
        bDir = CreateDirectory(
            pathToCreate.c_str(),
            NULL);
        if (bDir == 0)
        {
            cout << " CreateDirectory Failed & Error No - " << GetLastError() << endl;
        }
        else
        {
            cout << "Directory logs\\incoming_message was successfully completed" << endl;
        }
        string incomingMessage = pathToLogs + "\\incoming_messages";
        string alertsMessages = pathToLogs + "\\alerts";
        string miscErrors = pathToLogs + "\\misc_errors";
        string networkFailure = pathToLogs + "\\network_failure";
        createFolder(incomingMessage);
        createFolder(alertsMessages);
        createFolder(miscErrors);
        createFolder(networkFailure);
    }
}

/*This function first checks to see if the directories exist before creating them*/
// reference
// https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
//https://www.youtube.com/watch?v=B999K9yztnI
bool Base_Unit::directoryExists(const std::string& directoryName)
{
    DWORD ftyp = GetFileAttributesA(directoryName.c_str());
    if (ftyp == INVALID_FILE_ATTRIBUTES)
    {
        return false;
    }
    if (ftyp & FILE_ATTRIBUTE_DIRECTORY)
    {
        return true;
    }
    return false;
}

/*This function allows the path to a logs directory to be set
 * The default set was C:\\logs 
 */
void Base_Unit::setPathToLogs(string& path)
{
    pathToLogs = path;
}

/*This function gets the path to the directory logs*/
string Base_Unit::getPathToLogs()
{
    return pathToLogs;
}

/*This function gets the path to the logs/incoming_messages directory*/
string Base_Unit::getPathToMessages()
{
    string messages = getPathToLogs() + "\\incoming_messages\\";
    return messages;
}

/* This function gets the path to the logs/alerts directory*/
string Base_Unit::getPathToAlerts()
{
    string messages = getPathToLogs() + "\\alerts\\";
    return messages;
}

/* This function gets the path to the logs/network_failure directory*/
string Base_Unit::getPathToNetWorkFailure()
{
    string messages = getPathToLogs() + "\\network_failure\\";
    return messages;
    
}

/* This functions gets the path to the logs/misc_errors directory*/
string Base_Unit::getPathToMiscErrors()
{
    string messages = getPathToLogs() + "\\misc_errors\\";
    return messages;
}

/*This functions returns the name of the current file that is open for a
 * specific type.  For example, if you needed to append to an alert log, you
 * would get the name of the current file that is open for that type using this
 * function.  */
string Base_Unit::getCurrentFileName(int type)
{
    if (type == 0)
    {
        return logFileName;
    }
    else if (type == 1)
    {
        return alertFile;
    }
    else if (type == 2)
    {
        return netFailFile;
    }
    else
    {
        return miscErrorFile;
    }
}
/* This function sets the file name for a log file when it is newly opened. 
 *  An example of it's use would be if a previous log file was full and 
 * closed and a new name created for a new file, then this function would
 * be called to set the file name for that particular type of log file. 
 */
void Base_Unit::setFileName(int type)
{
    if (type == 0)
    {
        string tempName;
        createFileName(&tempName, type);
        logFileName = tempName;
    }
    else if (type == 1)
    {
        string tempName;
        createFileName(&tempName, type);
        alertFile = tempName;
    }
    else if (type == 2)
    {
        string tempName;
        createFileName(&tempName, type);
        netFailFile = tempName;
    }
    else
    {
        string tempName;
        createFileName(&tempName, type);
        miscErrorFile = tempName;
    }
}




