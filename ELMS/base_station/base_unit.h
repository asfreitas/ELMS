/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#ifndef BASE_UNIT_H
#define BASE_UNIT_H

#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/types/bson_value/view.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <bsoncxx/types.hpp>

#include <thread>
#include <time.h>


#include "parse_incoming.h"
#include "outgoing.h"
#include "utilities.h"
#include "vehicle.h"
#include "calculations.h"
#include "port.h"
#include "fileio.h"
#include "connect_database.h"

#include <iomanip>

#define MESSAGE_LIMIT 5
#define SECONDS_LIMIT 5

using std::string;
using std::vector;
using std::map;
using std::pair;
using std::setprecision;
using std::iterator;

/* reference for using statement:
 *  https://jira.mongodb.org/browse/CXX-860
*/
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using std::chrono::milliseconds;


class Base_Unit
{
    // using static variables will be the same value for all class objects.

    //The vector of Vehicles functions as a "master list" of all the vehicles
    // that are currently in the mine
    static vector<Vehicle*>mine_vehicles;
    FileIO fileHandler;
    Database database;

    //this is the path to the folder that stores the logs. It has a set function
    // to allow the user to create the folder wherever they want.  
public:

    //Constructors
    Base_Unit();

    // class destructor will manage memory leak by deleting pointings to
    // vehicle objects in the mine_vehicles vector.
    ~Base_Unit();

    void logToFile(std::string, MessageType);
    void addToMineVehicles(Vehicle* v);
    void print_vector(vector<Vehicle*> v);
    vector<Vehicle*>getMineVehicles();

    /* returns the size of the mine_vehicle vector*/
    size_t get_size(const vector<Vehicle*>& v);

    /*Set Functions for the Master Vector Containing Vehicle Objects */
    void setVehicleInMineVehicles(int indice, int time, double latitude, double longitude,
        double velocity, double bearing, int priority, string status);
    void setVehicleInMineVehicles2(Vehicle* v, int time, double latitude, double longitude,
        double velocity, double bearing, int priority, string status);

    /* Functions to input, update, check for vehicle id's and distances from
     * other vehicles */
    void input_data(int index, struct message* ptr, Port& p, HANDLE& h);
    void update_data(struct message* ptr, int indice, int &newVehicle);
    int contains_id_number(int id, unsigned long int& index);
    map<int, double> checkDistancesInMasterVector1(Vehicle* v);
    int checkOtherVehiclesPriorityNumbers(Vehicle* v1, int index, int priority_number);

    //constructor for Base_Unit
    // Reference: https://www.cplusplus.com/forum/beginner/34589/
    //Base_Unit() : fileHandler("C:\\logs", MESSAGE_LIMIT), database("mongodb + srv://asfreitas:b8_i7miJdVLAHFN@elms-cluster-k27n4.gcp.mongodb.net/test?retryWrites=true&w=majority") {};
};

#endif // !BASE_UNIT_H