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
#include "parse_incoming.h"
#include "outgoing.h"
#include "utilities.h"
#include "vehicle.h"
#include "parse_incoming.h"
#include "calculations.h"
#include "port.h"
#include <algorithm>
//#include <iterator>
using std::string;
using std::vector;
using std::setprecision;

//this is how many messages will be in each log file before it is close. 
// this is easily changed and we can make any number we want. 
#define MESSAGE_LIMIT 5

static vector<Vehicle>mine_vehicles;
static vector<Vehicle>priority_list;
class Base_Unit
{
    // using static variables will be the same value for all class objects.

    //The vector of Vehicles functions as a "master list" of all the vehicles
    // that are currently in the mine
    //static vector<Vehicle>mine_vehicles;
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
    void input_data(struct message* ptr, Vehicle& v, vector<Vehicle>& mineVehicles, Port& p);
    void update_data(struct message* ptr, Vehicle& v, vector<Vehicle>& mineVehicles);
    int get_size(vector<Vehicle>& v);
    int contains_id_number(vector<Vehicle>& v, int id, int& index);
    vector<int> checkDistancesInMasterVector(Vehicle& v, double& d);
    void updateMasterPriority(Vehicle& v);
};

#endif // !BASE_UNIT_H