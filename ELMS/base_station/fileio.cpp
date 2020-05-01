/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

#include "fileio.h"


FileIO::FileIO(std::string path, int limit)
{
    pathToLogs = path;
    messageLimit = limit;
    // setup initial directory locations
    pathToMessages = pathToLogs + "\\incoming_messages\\";
    pathToAlerts = pathToLogs + "\\alerts\\";
    pathToNetworkFailure = pathToLogs + "\\network_failure\\";
    pathToMiscErrors = pathToLogs + "\\misc_errors\\";

    createFolders();
}
FileIO::FileIO() 
{
    createFolders();

    pathToMessages = pathToLogs + "\\incoming_messages\\";
    pathToAlerts = pathToLogs + "\\alerts\\";
    pathToNetworkFailure = pathToLogs + "\\network_failure\\";
    pathToMiscErrors = pathToLogs + "\\misc_errors\\";
} 
FileIO::~FileIO()
{
    // do something
}

bool FileIO::checkMessageCount(MessageType type)
{
    int& messageCount = getMessageCount(type); // get it by reference so we can increment it after
    if (messageCount < messageLimit)
    {
        messageCount++; // increment the message type that is passed into the function
        return false;
    }
    return true;
}

int& FileIO::getMessageCount(MessageType type)
{
    switch (type)
    {
    case MessageType::incoming: return messageCount;
    case MessageType::alert: return alertCount;
    case MessageType::network_failure: return networkFailureCount;
    case MessageType::misc: return miscCount;
    default: break;
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
 * Important to use a reference wrapper when passing in threads.
 * https://stackoverflow.com/questions/34078208/passing-object-by-reference-to-stdthread-in-c11
 */
void FileIO::logToFile(std::string fileName, std::string inputMessage, MessageType type)
{
    bool needNewFile = false;

    // flag that will be true when both writing and closing to a file have finished. 
    bool done = false;

    //check to see if the number of messages written to the file is at the limit
    // It checks for the message type to determine if more should be written to
    // that particular log file. 
    if (writing_thread.joinable())
        writing_thread.join(); // check to join previous write before wriing again 
    // in the future we can setup a thread pool for a potential speedup
    writing_thread = std::thread(&FileIO::lockWriteFile, this, fileName, inputMessage);

    bool messageCountAtLimit = checkMessageCount(type);

    if (messageCountAtLimit)
    {
        getNewFilePath(fileName, type);
    }
}

/* Function lockCloseFile
 * This function locks while closing a file so that it can be stored in its folder
 * type = 0 means that it is an incoming message
 * type = 1 means that it is an alert being sent to a vehicle
 * type = 2 means that it is a network failure message
 * type = 3 means that it is a miscellaneous error.
 * Reference: https://www.daniweb.com/programming/software-development/threads/476954/convert-from-localtime-to-localtime-s
 */

 /* Function lockWriteFile locks any other part of the program writing to
  * log files while another process is writing to it. */
void FileIO::lockWriteFile(string filePath, string inputMessage)
{
    //lock writing to a log file
    mtx_write.lock();

    writeToFile(filePath, inputMessage);
    //open the lock
    mtx_write.unlock();

}
/* This function sets the file name for a log file when it is newly opened.
 *  An example of it's use would be if a previous log file was full and
 * closed and a new name created for a new file, then this function would
 * be called to set the file name for that particular type of log file.
 */
void FileIO::setFileName(MessageType type)
{
    std::string tempName = createFileName(type);

    switch (type)
    {
    case MessageType::incoming: logFileName = tempName; break;
    case MessageType::alert: alertFile = tempName; break;
    case MessageType::network_failure: netFailFile = tempName; break;
    case MessageType::misc: miscErrorFile = tempName; break;
    default: break;
    }
}

void FileIO::resetMessageCount(MessageType type)
{
    switch (type)
    {
    case MessageType::incoming: messageCount = 1; break;
    case MessageType::alert: alertCount = 1; break;
    case MessageType::network_failure: networkFailureCount = 1; break;
    case MessageType::misc: miscErrorFile = miscCount = 1; break;
    default: break;
    }
}

std::string FileIO::createFileName(MessageType type)
{
    resetMessageCount(type);
    std::string type_of_message;

    switch (type)
    {
    case MessageType::incoming: type_of_message = "I"; break;
    case MessageType::alert: type_of_message = "A"; break;
    case MessageType::network_failure: type_of_message = "N"; break;
    case MessageType::misc: type_of_message = "M"; break;
    default: break;
    }

    //get the current time
    time_t now = time(0);

    //declare a time structure
    struct tm gmtm;

    //use a thread safe call to get the current UTC time
    gmtime_s(&gmtm, &now);

    int year = gmtm.tm_year + 1900;
    int month = gmtm.tm_mon + 1;
    int day = gmtm.tm_mday;
    int hour = gmtm.tm_hour;
    int min = gmtm.tm_min;
    int sec = gmtm.tm_sec;

    //build the string needed for the name of the file. 
    std::string tempName = std::to_string(year) + '_' + std::to_string(month) + '_' + std::to_string(day) + '_' +
        std::to_string(hour) + '_' + std::to_string(min) + '_' + std::to_string(sec) + '_' + type_of_message;

    //add to the end of the string the file extension type. 
    tempName = tempName + ".txt";

    return tempName;

}


/*This function first checks to see if a directory exists, then it creates the
 * logs directory and the 4 subfolders (1) incoming_messages (2) alerts (3) network_failure
 * (4) misc_errors. It will create the main logs file in the location that the user
 * specified. */
 // https://www.youtube.com/watch?v=B999K9yztnI
void FileIO::createFolders()
{
    // create various folders for use in program

    createFolder(pathToLogs);
    createFolder(pathToAlerts);
    createFolder(pathToMessages);
    createFolder(pathToMiscErrors);
    createFolder(pathToNetworkFailure);
}

bool FileIO::createFolder(const string folderName)
{
    bool exists;
    int directory;
    exists = directoryExists(folderName);

    // if the exists is false, then create the directory
    if (!exists)
    {
        directory = CreateDirectory(folderName.c_str(), NULL);

        // if an error occurs creating the directory then notify the user
        if (directory == 0)
        {
            //TO DO: use GetLastError(), format it and determine what the actual
           // error is. 
            cout << " CreateDirectory Failed. The error number is:  " << GetLastError() << endl;
        }

        // otherwise let the user know the directory was successfully made.
        else
        {
            cout << "Directory" << folderName << " was successfully created" << endl;

        }
        return false;
    }
    return true;
}

/*This function first checks to see if the directories exist before creating them
 * The GetFileAttributesA function returns attributes for specific file or
 * directory. If the call is successful, then the attribute is returned.
 * In this case we are looking for it to return "ERROR_PATH_NOT_FOUND" ,
 * "ERROR_FILE_NOT_FOUND", "ERROR_INVALID_NAME", or "ERROR_BAD_NETPATH" in order
 * to know that the file does not exist. */
 // reference
 // https://stackoverflow.com/questions/8233842/how-to-check-if-directory-exist-using-c-and-winapi
 //https://www.youtube.com/watch?v=B999K9yztnI
 //https://docs.microsoft.com/en-us/windows/win32/api/fileapi/nf-fileapi-getfileattributesa
bool FileIO::directoryExists(const std::string& directoryName)
{
    DWORD directory_attribute = GetFileAttributesA(directoryName.c_str());
    if (directory_attribute == INVALID_FILE_ATTRIBUTES)
    {
        //option to use GetLastError(), format it and determine what the actual
        // error is. 
        return false;
    }
    //if the directory attribute returns > 0, then this means the directory
    // exists and we return true. 
    if (directory_attribute & FILE_ATTRIBUTE_DIRECTORY)
    {
        return true;
    }
    return false;
}

void FileIO::getNewFilePath(string fileName,MessageType type)
{
    string newFilename = createFileName(type);
    switch (type)
    {
    case MessageType::incoming: logFileName = newFilename + fileName; break;
    case MessageType::alert: alertFile = newFilename + fileName; break;
    case MessageType::network_failure: netFailFile =  newFilename + fileName; break;
    case MessageType::misc: miscErrorFile = newFilename + fileName; break;
    default: break;
    }
}

void FileIO::writeToFile(string filePath, string message)
{
    std::ofstream inputFile;
    inputFile.open(filePath, std::ios::out | std::ios::app);

    // if the log file is open, then write to it. 
    if (inputFile.is_open())
    {
        inputFile << message;
    }
    inputFile.close();
    
}

