/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#ifndef BASE_UNIT_H
#define BASE_UNIT_H


#include <iostream>
#include <fstream>
#include <istream>
#include <string>
#include <iomanip>
#include <Windows.h>
#include <vector>
#include <algorithm>
#include <map>
#include <iterator> 

#include "parse_incoming.h"
#include "outgoing.h"
#include "utilities.h"
#include "vehicle.h"
#include "calculations.h"
#include "port.h"
<<<<<<< HEAD
#include <algorithm>
#include <map>
#include <iterator>
||||||| merged common ancestors
#include <algorithm>
#include <map>
#include <iterator>
//#include <iterator>
=======
#include "fileio.h"

//#include <iterator>
>>>>>>> origin/FileHandle
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::setprecision;
using std::iterator;

//this is how many messages will be in each log file before it is close. 
// this is easily changed and we can make any number we want. 
#define MESSAGE_LIMIT 5

static vector<Vehicle>priority_list;

class Base_Unit
{
    // using static variables will be the same value for all class objects.

    //The vector of Vehicles functions as a "master list" of all the vehicles
    // that are currently in the mine
    //static vector<Vehicle>mine_vehicles;
<<<<<<< HEAD
    static int messageCount;
    static int alertCount;
    static int networkCount;
    static int miscCount;
    static string logFileName;
    static string alertFile;
    static string netFailFile;
    static string miscErrorFile;
    static vector<Vehicle*>mine_vehicles;
||||||| merged common ancestors
    static int messageCount;
    static int alertCount;
    static int networkCount;
    static int miscCount;
    static string logFileName;
    static string alertFile;
    static string netFailFile;
    static string miscErrorFile;
    static vector<Vehicle>mine_vehicles;
=======
    static vector<Vehicle>mine_vehicles;
    FileIO fileHandler;

>>>>>>> origin/FileHandle

    //this is the path to the folder that stores the logs. It has a set function
    // to allow the user to create the folder wherever they want.  
public:
<<<<<<< HEAD
    ~Base_Unit();

    /* Log File Functions*/
    bool checkMessageCount(int type);
    void logFile(string& fileName, string* inputMessage, int type);
    void lockWriteFile(string& filePath, string* inputMessage);
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
    void getFilePath(string& fileName, int type);

    /* mine_vehicle functions */
    void addToMineVehicles(Vehicle *v);
    /* Print Function*/
    void print_vector(vector<Vehicle*> v);

    /* Get Functions for the Master Vector Containing Vehicle Objects*/
    vector<Vehicle*> getMineVehicles();
    int get_size(vector<Vehicle*>& v);

    /*Set Functions for the Master Vector Containing Vehicle Objects */
    void setVehicleInMineVehicles(int indice, int time, double latitude, double longitude,
        double velocity, double bearing, int priority);
    void setVehicleInMineVehicles2(Vehicle* v, int time, double latitude, double longitude,
        double velocity, double bearing, int priority);
    
    /* Functions to input, update, check for vehicle id's and distances from
     * other vehicles */
||||||| merged common ancestors
    bool checkMessageCount(int type);
    void logFile(string& fileName, string* inputMessage, int type);
    void lockWriteFile(string& filePath, string* inputMessage);
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
    void getFilePath(string& fileName, int type);
    void addToMineVehicles(Vehicle& v);
    void print_vector(vector<Vehicle>& v);
    vector<Vehicle>getMineVehicles();
    void addToPriorityQueue(Vehicle& v);
    vector<Vehicle>getPriorityQueue();
=======
    void logToFile(std::string, MessageType);
    void addToMineVehicles(Vehicle& v);
    void print_vector(vector<Vehicle>& v);
    vector<Vehicle>getMineVehicles();
    void addToPriorityQueue(Vehicle& v);
    vector<Vehicle>getPriorityQueue();
>>>>>>> origin/FileHandle
    void input_data(int index, struct message* ptr,Port& p, HANDLE& h);
    void update_data(struct message* ptr, int indice);
<<<<<<< HEAD
||||||| merged common ancestors
    int get_size(vector<Vehicle>& v);
=======
    size_t get_size(const vector<Vehicle>& v);
>>>>>>> origin/FileHandle
    int contains_id_number(int id, int& index);
<<<<<<< HEAD
    map<int, double> checkDistancesInMasterVector1(Vehicle* v);
    int checkOtherVehiclesPriorityNumbers(Vehicle* v1, int index, int priority_number);


||||||| merged common ancestors
    map<int, double> checkDistancesInMasterVector(Vehicle& v);
    void updateMasterPriority(Vehicle& v);
    void setVehicleInMineVehicles1(Vehicle &, int time, double latitude, double longitude,
        double velocity, double bearing, int priority);
    void setVehicleInMineVehicles(int indice, int time, double latitude, double longitude,
       double velocity, double bearing, int priority);
=======
    map<int, double> checkDistancesInMasterVector(Vehicle& v);
    void updateMasterPriority(Vehicle& v);
    void setVehicleInMineVehicles1(Vehicle &, int time, double latitude, double longitude,
    double velocity, double bearing, int priority);
    void setVehicleInMineVehicles(int indice, int time, double latitude, double longitude,
    double velocity, double bearing, int priority);
    string getPathToMessages() { return fileHandler.getPathToMessages(); }
    string getPathToLogs() { return fileHandler.getPathToLogs(); }
    string getPathToLogFile() { return fileHandler.getLogFilePath(); }
    Base_Unit() : fileHandler("C:\\logs", MESSAGE_LIMIT) {}; // https://stackoverflow.com/questions/849812/c-construction-of-an-object-inside-a-class

>>>>>>> origin/FileHandle
};

#endif // !BASE_UNIT_H