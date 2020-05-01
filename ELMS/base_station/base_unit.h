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
#include "fileio.h"

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
    static string logFileName;
    static string alertFile;
    static string netFailFile;
    static string miscErrorFile;
    static vector<Vehicle>mine_vehicles;
    FileIO fileHandler;


    //this is the path to the folder that stores the logs. It has a set function
    // to allow the user to create the folder wherever they want.  
public:
    void logToFile(std::string, std::string, MessageType);
    void addToMineVehicles(Vehicle& v);
    void print_vector(vector<Vehicle>& v);
    vector<Vehicle>getMineVehicles();
    void addToPriorityQueue(Vehicle& v);
    vector<Vehicle>getPriorityQueue();
    void input_data(int index, struct message* ptr,Port& p, HANDLE& h);
    void update_data(struct message* ptr, int indice);
    int get_size(vector<Vehicle>& v);
    int contains_id_number(int id, int& index);
    map<int, double> checkDistancesInMasterVector(Vehicle& v);
    void updateMasterPriority(Vehicle& v);
    void setVehicleInMineVehicles1(Vehicle &, int time, double latitude, double longitude,
    double velocity, double bearing, int priority);
    void setVehicleInMineVehicles(int indice, int time, double latitude, double longitude,
    double velocity, double bearing, int priority);
    string getPathToMessages() { return fileHandler.getPathToMessages(); }
    string getPathToLogs() { return fileHandler.getPathToLogs(); }
    string getPathToLogFile() { return fileHandler.getLogFilePath(); }
    Base_Unit() : fileHandler("C:\\", 5) {}; // https://stackoverflow.com/questions/849812/c-construction-of-an-object-inside-a-class

};

#endif // !BASE_UNIT_H