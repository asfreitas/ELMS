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
#include <map>
#include <iterator>
//#include <iterator>
using std::string;
using std::vector;
using std::map;
using std::pair;
using std::setprecision;

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
    static int messageCount;
    static int alertCount;
    static int networkCount;
    static int miscCount;
    static string logFileName;
    static string alertFile;
    static string netFailFile;
    static string miscErrorFile;
    static vector<Vehicle>mine_vehicles;
    static vector<Vehicle*>mine_vehicles1;

    //this is the path to the folder that stores the logs. It has a set function
    // to allow the user to create the folder wherever they want.  
    string pathToLogs = "C:\\logs";
public:
    ~Base_Unit();

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
    void addToMineVehicles1(Vehicle *v);

    void print_vector(vector<Vehicle>& v);
    void print_vector1(vector<Vehicle*> v);


    vector<Vehicle>getMineVehicles();
    vector<Vehicle*> getMineVehicles1();
    void addToPriorityQueue(Vehicle& v);
    vector<Vehicle>getPriorityQueue();
    void input_data(int index, struct message* ptr,Port& p, HANDLE& h);

    void update_data(struct message* ptr, int indice);
    int get_size(vector<Vehicle>& v);
    int get_size1(vector<Vehicle*>& v);

    int contains_id_number(int id, int& index);
    int contains_id_number1(int id, int& index);
    map<int, double> checkDistancesInMasterVector(Vehicle& v);
    map<int, double> checkDistancesInMasterVector1(Vehicle* v);
    void updateMasterPriority(Vehicle& v);
    void setVehicleInMineVehicles1(Vehicle &, int time, double latitude, double longitude,
        double velocity, double bearing, int priority);
    void setVehicleInMineVehicles(int indice, int time, double latitude, double longitude,
       double velocity, double bearing, int priority);
    void setVehicleInMineVehicles2(Vehicle *v, int time, double latitude, double longitude,
        double velocity, double bearing, int priority);
};

#endif // !BASE_UNIT_H