/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#include "vehicle.h"
#include <iostream>
#include <stdbool.h>
#include <thread>
#include <mutex>
#include <fstream>
#include <string>
using std::thread;

// declare a mutex that locks when the program is writing to a log file
std::mutex mtx_close;
std::mutex mtx_write;

// this is just a temporary declaration of a log file that will be used in the
// logFile function
std::ifstream logFile;

/* In order for the mutex to lock while the program is writing to a file, the
 * Vehicle class needs to have class members that are static. Static member 
 * will then share the same value for all class objects. These static class
 * members are:
 * (1) int messageCount - it counts the number of messages receive. It is initialized to 0
 * (2) a bool called fileOpen that is initialized to false. False means that a
       log file is not currently open and being written to.  Whenver fileOpen is
       false, a new file should be opened and then written to. If fileOpen is true
       then when a new message is receive, just append to the log file that is open. 
 */

/* Also important that before opening a new log file, the fileOpen flag should first 
 * be checked by using the getter function getFileOpen() function. This function
 * returns the current value of the bool flag fileOpen. 
 */

/* The flow
 * 1. Before data is parsed and stored in the Vehicle object and it is still either
 * a string or char *
 * 2. Call the function logFile and send the whole string or char * inputMessage
 * 3. logFile first checks the messageCount
 * 4. If messageCount >= 1000, logFile declares a thread that calls the function lockWriting.
 * 5. lockWriting locks the mutex and writes the last message
 * 6. lockWriting is unlocked returns and the thread joins. 
 * 7. lockClose is called and the mutex is locked
 * 8. The logFile is closed and stored in the local file system (not uploaded to 
 *     database yet)
 * 9. setFileOpen() is called and will set the flag to false -- meaning there is not
 *     a log file open
 * 10. lockClose then unlocks the mutex and returns to logFile()
 * 11. logFile thread t2 then joins. 
 * 12. If the messageCount was < 1000, it was incremented and then calls the
 *     lockWrite function to lock while writing to the log file. 
 * 13. Once the log file has been created, the Vehicle class can then proceed with
       parsing the data and loading it into the Vehicles. 

/*
 * Function: checkMessageCount(int messageCount)
 * This function takes the class static variable messageCount and checks to see
 * if it has reached 1000 messages. If message count is < 1000, increment the
 * message count and return false.
 * Otherwise, true is returned. 
*/
bool Vehicle::checkMessageCount()
{
    if (getMessageCount() < 1000)
    {
        messageCount++;
    }
    else
    {
        return true;
        
    }
}

/*
 * Function logFile
 * This function checks the message count. If it is < 1000, then the inputMessage
 * is written to a log file. and the message count is incremented. If the 
 * message count is >= 1000, then this last message is written to the log file
 * and the log file is closed.  This log file will be stored in the local 
 * directory and at regular intervals uploaded to the database.  When it is 
 * closed it will be saved with a name created by using the Zulu time. 
 * References: https://www.cplusplus.com/reference/mutex/mutex/lock/
*/
void Vehicle::logFile(std::fstream *logFile, char * inputMessage)
{
    // flag that will be true when both writing and closing to a file have finished. 
    bool done = false;
    //check to see if the number of messages written to the file is 1000
    bool messageCountAtLimit = checkMessageCount();

    //this loop while continue until both the writing and the closing of the 
    // log file have finished.  It prevents another message trying to write to
    // a file that is being closed. 
    while (messageCountAtLimit && done == false)
    {
        //declare a thread that will write this last message to the log file
        thread t1(&lockWriteFile, logFile);
        
        // have the thread join
        t1.join();
        // declare a thread and call the function to lock writing to the log file
        thread t2(&lockCloseFile, logFile);

        //have the thread join again
        t2.join();
        done = true;
    }

    //messageCount was less than 1000 and the count was incremented. 
    // Call the function that writes the data to the current log file.
    if(!messageCount)
    {
        thread t1(&lockWriteFile, logFile);
        
        // have the thread join again
        t1.join();
    }
}

/* Function lockCloseFile
 * This function locks while closing a file*/
void Vehicle::lockCloseFile(std::fstream *logFile)
{
    std::lock_guard<std::mutex> lck{ mtx_close };
    logFile->close();

    /* placeholder to write the function that will store the log file in a local directory
     * This file will be renamed as stored as the current time*/

     //change the fileOpen flag to false so that a new file will be created
     // with the next message
    setFileOpen();

    // reset the messageCounter to 0;
    resetMessageCount();

    //don't need to unlock because using lock_guard

    return;
}

/* Function lockWriteFile locks any other part of the program writing to
 * log files while another process is writing to it. */
void Vehicle::lockWriteFile(std::fstream* logFile, char * inputMessage)
{
    //lock writing to a log file
    mtx_write.lock();

    //call the function that write the input string or char * to the log file
    // PLACEHOLDER
    
    //open the lock
    mtx_write.unlock();
    return;

}

/*Function is a getter for messageCount */
int Vehicle::getMessageCount()
{
    return messageCount;
}

/* Function resets messageCount to 0 when a new log file is opened*/
void Vehicle::resetMessageCount()
{
    messageCount = 0;
}

/* Function is a getter for the bool fileOpen*/
bool Vehicle::getFileOpen()
{
    return fileOpen;
}

/* This is a setter function for the value of the bool flag fileOpen*/
void Vehicle::setFileOpen()
{
    if (getFileOpen() == true)
    {
        fileOpen = false;
    }
    else
    {
        fileOpen = true;
    }

}

