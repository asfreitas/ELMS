/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

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
#include <time.h>

using std::thread;
using std::cout;
using std::endl;

// declare a mutex that locks when the program is writing to a log file
std::mutex mtx_close;
std::mutex mtx_update_master;
std::mutex mtx_udate_vehicle;

/* Static members must be initialized outside of the class
 * Since it is a static member, it must be initialized outside of the class
 */


vector<Vehicle> Base_Unit::mine_vehicles;




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

 * type = 0 means that it is an incoming message
 * type = 1 means that it is an alert being sent to a vehicle
 * type = 2 means that it is a network failure message
 * type = 3 means that it is a miscellaneous error.
 *
 * string fileName is created by using b.getFileName(&fileName, 0); where b
 * is a Base_Unit object and the second parameter is the message type
*/

void Base_Unit::addToMineVehicles(Vehicle& v)
{
    mine_vehicles.push_back(v);
}

/* prints the contents held in the mine_vehicle vector*/
void Base_Unit::print_vector(vector<Vehicle>& v)
{
    if (v.size() == 0)
        return;
    //cout << "The size of the vector in print function is: " << v.size() << endl;
    cout << "**********************************" << endl << endl;

    for (auto itr : v)
    {
        cout << "*****VEHICLE " << itr.getUnit() << " *****" << endl;
        cout << std::fixed;
        cout << std::setprecision(5);
        cout << "latitude: " << itr.getLatitude() << endl;
        cout << "longitude: " << itr.getLongitude() << endl;
        cout << std::fixed;
        cout << std::setprecision(0);
        cout << "time_stamp: " << itr.getTime() << endl;
        cout << "velocity: " << itr.getVelocity() << endl;
        cout << "bearing: " << itr.getBearing() << endl;
        cout << "priority: " << itr.getPriorityNumber() << endl << endl;
    }
    cout << "***************************************" << endl << endl;

}

vector<Vehicle> Base_Unit::getMineVehicles()
{
    return mine_vehicles;
}

void Base_Unit::addToPriorityQueue(Vehicle& v)
{
    priority_list.push_back(v);
}

vector<Vehicle> Base_Unit::getPriorityQueue()
{
    return priority_list;
}

/* This function takes the new message and either inputs a new vehicle or 
 * updates a current one The reference to use maps is the following:
 * https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/  
 */
void Base_Unit::input_data(int indice, struct message* ptr, Port& p, HANDLE& h)
{
    Calculations calc;
    int size = -1;
    string alertMessage;
    string alertLogMessage;
    string alertFileName;
    char fileName[100];
    double speed, distance = 0, bearing;
    int index;
    //this map holds vehicles that are within 50 meters or less of the input
    // vehicle.  The int is the index in the master list of that vehicle and
    // the double is the distance. 
    map<int, double>::iterator itr;

    size = get_size(mine_vehicles);
    //if the size of the vector is 0, then we know we need to add the 
    // vehicle to the vector
    if (size == 0)
    {

        mine_vehicles.at(indice).setBearing(ptr->bearing);
        mine_vehicles.at(indice).setLatitude(ptr->latitude);
        mine_vehicles.at(indice).setLongitude(ptr->longitude);
        mine_vehicles.at(indice).setTime(ptr->time);
        mine_vehicles.at(indice).setUnit(ptr->vehicle);
        mine_vehicles.at(indice).setVelocity(ptr->velocity);
        mine_vehicles.at(indice).setPriority(3);

    }
    // otherwise, we check to see if the vehicle id already exists
    else
    {
        // call update_data
        thread t1 = std::thread(&Base_Unit::update_data, this, ptr, indice);

        // have the thread join again
        t1.join();

        //now the newly added node or updated node needs to have their distance
        // checked to all the other nodes. 
        //need to check for the index again where the vehicles is.
        contains_id_number(ptr->vehicle, index);
        map<int, double>list = checkDistancesInMasterVector(mine_vehicles.at(index));
        //next we iterate through the map with the distances and send out alerts
        if (list.size() > 0)
        {
            for(itr = list.begin(); itr != list.end(); ++ itr)
            {
                //need to send out an alert message
                // first calculate the speed which is in meters per second
                speed = calc.knots_to_mps(ptr->velocity);

                //The list contains the index in the master list of vehicles that is
                // in immediate danger of collision. Set the second vehicle to this
                // index
                Vehicle v1 = mine_vehicles.at(itr->first);

                // now we get the bearing
                bearing = calc.getBearing(&mine_vehicles.at(index), &v1);
                // we create the outgoing message;
                outgoing_message(alertMessage, ptr->vehicle, v1.getUnit(), ptr->time, speed,
                    itr->second, bearing);

                //copy the message to use to create the log file for the alert
                alertLogMessage = alertMessage;
                //convert the outgoing message to a char * so that it can be transmitted
                stringToCharPointer(alertMessage, fileName);
                // send the alerts
                DWORD test = p.writeToSerialPort(fileName, alertMessage.length() + 1, h);
                
                //we also need to create the alert message
                // remove the \n from the end of the message
                alertLogMessage = alertLogMessage.substr(0, alertLogMessage.size() - 2);
                //get the file path
                //getFilePath(alertFileName, 1);
                // call logFile to create the log file
                fileHandler.logToFile(alertLogMessage, MessageType::alert);
                //logFile(alertFileName, &alertLogMessage, 1);
            }
        }

        // now we need to sort the mine_vehicles
        Vehicle v;
        v.sortVehicleVector(mine_vehicles);

        //now we need to add these vehicles to each other's priority queues as well
        // as add the vehicles to the master unit's priority queue. 
        //addToPriorityQueue(v);

    }

}
void Base_Unit::update_data(struct message* ptr, int indice)
{
    mtx_update_master.lock();
    int duplicate, index;
    //check for a duplicate id
    duplicate = contains_id_number(ptr->vehicle, index);
 
    //if duplicate is 0, this means that this is a new vehicle
    if (duplicate == 0)
    {
        mine_vehicles.at(indice).setBearing(ptr->bearing);
        mine_vehicles.at(indice).setLatitude(ptr->latitude);
        mine_vehicles.at(indice).setLongitude(ptr->longitude);
        mine_vehicles.at(indice).setTime(ptr->time);
        mine_vehicles.at(indice).setUnit(ptr->vehicle);
        mine_vehicles.at(indice).setVelocity(ptr->velocity);
        mine_vehicles.at(indice).setPriority(3);
    }
    //otherwise, this is a duplicate id and we need to update
    // the Vehicle objects location data
    else
    {
        //first we need to erase the duplicate we added to the end of the
        // vector and then just update the existing vehicle at its index
        size_t size = mine_vehicles.size();
        mine_vehicles.erase(mine_vehicles.begin() + mine_vehicles.size() - 1);
        setVehicleInMineVehicles(index, ptr->time, ptr->latitude, ptr->longitude, ptr->velocity, ptr->bearing, -1);
    }
    mtx_update_master.unlock();

}
// returns the size of the vector
size_t Base_Unit::get_size(const vector<Vehicle>& v)
{
    return v.size();
}

//check to see if vector contains a specific id number
int Base_Unit::contains_id_number(int id, int& index)
{
    for (size_t i = 0; i < getMineVehicles().size(); i++)
    {
        if (getMineVehicles().at(i).getUnit() == id)
        {
            index = i;
            //cout << "the index is: " << index << endl;
            return 1;
        }
    }
    return 0;
}

/* function iterates through the master list and checks the distance each
 * vehicle is from the other vehicles, it adds any vehicles that are close to that
 * input vehicle to a vector that contains ints which represent the index number
 * in the vector of vehicles where there is risk of collision*/
//Reference for maps: https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
map<int,double> Base_Unit::checkDistancesInMasterVector(Vehicle& v)
{
    Calculations c;
    map<int, double> listOfAlerts;
    bool set = false;
    double distance;
    
    //iterate through the master list of vehicles
    for (size_t i = 0; i < getMineVehicles().size(); i++)
    {
        if (mine_vehicles.at(i).getUnit() != v.getUnit())
        {
            // get the distance
            distance = c.haversine(&v, &mine_vehicles.at(i));
            //distance returns as km, need to change to meters so multiply
            // by 1000
            distance = 1000 * distance;
            //if the result is 50 or less than the index of the vehicle in the 
            // master list and it's distance is inserted into the map
            if (distance <= 50)
            {
                listOfAlerts.insert(pair<int, double>(i, distance));
                // set both of the vehicles priority numbers to 0
                setVehicleInMineVehicles(i, -1, -1, -1, -1, -1, 0);
                setVehicleInMineVehicles1(v, -1, -1, -1, -1, -1, 0);             
                // a 0 priority was set, so this needs to remain
                set = true;

            }
            else if (distance > 50 && distance <= 75 && !set)
            {
                
                //mining_vehicles.at(index).setPriority(1);
                setVehicleInMineVehicles1(v, -1, -1, -1, -1, -1, 1);
                setVehicleInMineVehicles(i, -1, -1, -1, -1, -1, 1);
                

            }
            else if (distance > 75 && distance < 100 && !set && v.getPriorityNumber()>2)
            {
                setVehicleInMineVehicles1(v, -1, -1, -1, -1, -1, 2);
                setVehicleInMineVehicles(i, -1, -1, -1, -1, -1, 1);
                
            }
            else if (distance >= 100 && !set && v.getPriorityNumber() > 3)
            {
                setVehicleInMineVehicles1(v, -1, -1, -1, -1, -1, 3);
                setVehicleInMineVehicles(i, -1, -1, -1, -1, -1, 3);
          
            }
        }
    }
    return listOfAlerts;
}

void Base_Unit::updateMasterPriority(Vehicle& v)
{
    for (size_t i = 0; i < mine_vehicles.size(); i++)
    {
        if (mine_vehicles.at(i).getUnit() == v.getUnit())
        {
            mine_vehicles.at(i) = v;
        }
    }
}

/* This updates the master vehical vector vehicles at a specific index.  If you only
 *  want to update one value, set the other inputs to -1 and nothing will be changed. */
void Base_Unit::setVehicleInMineVehicles(int index, int time, double latitude, double longitude, double velocity, double bearing, int priority)
{
    if (time != -1)
    {
        mine_vehicles.at(index).setTime(time);
    }
    if (latitude != -1)
    {
        mine_vehicles.at(index).setLatitude(latitude);
    }
    if (longitude != -1)
    {
        mine_vehicles.at(index).setLongitude(longitude);
    }
    if (velocity != -1)
    {
        mine_vehicles.at(index).setVelocity(velocity);
    }
    if (bearing != -1)
    {
        mine_vehicles.at(index).setBearing(bearing);
    }
    if (priority != -1)
    {
        mine_vehicles.at(index).setPriority(priority);
    }
}

void Base_Unit::setVehicleInMineVehicles1(Vehicle &v, int time, double latitude, double longitude, double velocity, double bearing, int priority)
{
    if (time != -1)
    {
        v.setTime(time);
    }
    if (latitude != -1)
    {
        v.setLatitude(latitude);
    }
    if (longitude != -1)
    {
        v.setLongitude(longitude);
    }
    if (velocity != -1)
    {
        v.setVelocity(velocity);
    }
    if (bearing != -1)
    {
        v.setBearing(bearing);
    }
    if (priority != -1)
    {
        v.setPriority(priority);
    }
}

void Base_Unit::logToFile(std::string message, MessageType type)
{
    fileHandler.logToFile(message, type);
}