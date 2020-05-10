/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#ifndef BASE_UNIT_H
#define BASE_UNIT_H

#define MESSAGE_LIMIT 5

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
#include <algorithm>
#include <map>
#include <iterator>
#include "fileio.h"

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::setprecision;
using std::iterator;


static vector<Vehicle>priority_list;

class Base_Unit
{
    // using static variables will be the same value for all class objects.

    //The vector of Vehicles functions as a "master list" of all the vehicles
    // that are currently in the mine
    //static vector<Vehicle>mine_vehicles;
    static vector<Vehicle*>mine_vehicles;
    FileIO* fileHandler;


    //this is the path to the folder that stores the logs. It has a set function
    // to allow the user to create the folder wherever they want.  
public:

    // class destructor will manage memory leak by deleting pointings to
    // vehicle objects in the mine_vehicles vector
    ~Base_Unit();
    Base_Unit(FileIO *_f);

    void logToFile(std::string, MessageType);
    void addToMineVehicles(Vehicle* v);
    void print_vector(vector<Vehicle*> v);
    vector<Vehicle*>getMineVehicles();
    void addToPriorityQueue(Vehicle& v);
    vector<Vehicle>getPriorityQueue();

    /* returns the size of the mine_vehicle vector*/
    size_t get_size(const vector<Vehicle*>& v);

    /*Set Functions for the Master Vector Containing Vehicle Objects */
    void setVehicleInMineVehicles(int indice, int time, double latitude, double longitude,
        double velocity, double bearing, int priority);
    void setVehicleInMineVehicles2(Vehicle* v, int time, double latitude, double longitude,
        double velocity, double bearing, int priority);

    /* Functions to input, update, check for vehicle id's and distances from
     * other vehicles */
    void input_data(int index, struct message* ptr, Port& p, HANDLE& h);
    void update_data(struct message* ptr, int indice);
    int contains_id_number(int id, unsigned long int& index);
    map<int, double> checkDistancesInMasterVector1(Vehicle* v);
    int checkOtherVehiclesPriorityNumbers(Vehicle* v1, int index, int priority_number);

   // Base_Unit() : fileHandler("C:\\logs", MESSAGE_LIMIT) {}; // https://stackoverflow.com/questions/849812/c-construction-of-an-object-inside-a-class
};

#endif // !BASE_UNIT_H