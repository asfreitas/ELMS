/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
 * Contains the function definitions for the class BASE_UNIT
*/

#include "base_unit.h"


//This string is the uri to the test database
string _uri = "mongodb+srv://asfreitas:b8_i7miJdVLAHFN@elms-cluster-k27n4.gcp.mongodb.net/test?retryWrites=true&w=majority";

// declare a mutex that locks when the program is writing to a log file
std::mutex mtx_close;
std::mutex mtx_update_master;
std::mutex mtx_udate_vehicle;

/* Static members must be initialized outside of the class
 * Since mine_vehicles is a static member, it must be initialized outside of
 * the class
 */
vector<Vehicle*> Base_Unit::mine_vehicles;

/*
===============
Base_Unit

Default Constructor for Base_Unit
Reference: https://www.cplusplus.com/forum/beginner/34589/
===============
*/
Base_Unit::Base_Unit() :  fileHandler("C:\\logs", MESSAGE_LIMIT), 
database(_uri)
{
    mine_vehicles = fileHandler.getSavedVehicles();
}

/*
===============
~Base_Unit

Destructor
deleteVector is a template struct that is defined in the utilities.h file. 
Reference for it's use is:
https://stackoverflow.com/questions/891913/c-stdvector-of-pointers-deletion-and-segmentation-faults#891924
===============
*/
Base_Unit::~Base_Unit()
{
    fileHandler.saveAllVehicles(mine_vehicles);

    std::for_each(mine_vehicles.begin(), mine_vehicles.end(), deleteVector());
}


/*
===============
addToMineVehicles

Function adds a vector pointer to the master vector of mine vehicles
===============
*/
void Base_Unit::addToMineVehicles(Vehicle* v)
{
    mine_vehicles.push_back(v);
}

/*
===============
print_vector

prints the contents held in the mine_vehicle vector
===============
*/
void Base_Unit::print_vector(vector<Vehicle*> v)
{
    //do not print anything if the vector is empty
    if (v.size() == 0)
        return;

    cout << "**********************************" << endl << endl;

    //this for-loop iterates through the vector and prints all the values
    for (auto itr : v)
    {
        cout << "*****VEHICLE " << itr->getUnit() << " *****" << endl;
        cout << std::fixed;
        cout << std::setprecision(5);
        cout << "latitude: " << itr->getLatitude() << endl;
        cout << "longitude: " << itr->getLongitude() << endl;
        cout << std::fixed;
        cout << std::setprecision(0);
        cout << "time_stamp: " << itr->getTime() << endl;
        cout << "velocity: " << itr->getVelocity() << endl;
        cout << "bearing: " << itr->getBearing() << endl;
        cout << "priority: " << itr->getPriorityNumber() << endl;
        cout << "status: " << itr->getStatus() << endl << endl;
        if (itr->getMapSize(*itr) > 0)
        {
            map<int, double>* mapVehicles = itr->getMapOfVehicles();
            printMap(mapVehicles);
            
        }
    }
    cout << "***************************************" << endl << endl;
  

}

/*
===============
getMineVehicles

returns the vector of pointers to the vehicles in the mine
===============
*/
vector<Vehicle*> Base_Unit::getMineVehicles()
{
    return mine_vehicles;
}


/*
===============
input_data

This function takes the new message and either inputs a new vehicle or
 updates a current one. The reference to use maps is the following:
 https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
===============
*/
void Base_Unit::input_data(int indice, struct message* ptr, Port& p, HANDLE& h)
{
    size_t size = -1;
    //strings used to create alert messages and log files
    string alertMessage;
    string alertMessage1;
    string alertLogMessage;
    string alertLogMessage1;
    string alertFileName;
    string alertFileName1;
    char fileName[100];
    char fileName1[100];
    unsigned long int index;
    //flag used to represent if a vehicle is new or already exists
    int newVehicle = 0;

    //iterator that will be used to iterate thru a map
    map<int, double>::iterator itr;
    size = get_size(mine_vehicles);
    //if the size of the vector is 0, then we know we need to add the 
    // vehicle to the vector
    if (size == 1)
    {

        mine_vehicles.at(indice)->setBearing(ptr->bearing);
        mine_vehicles.at(indice)->setLatitude(ptr->latitude);
        mine_vehicles.at(indice)->setLongitude(ptr->longitude);
        mine_vehicles.at(indice)->setTime(ptr->time);
        mine_vehicles.at(indice)->setUnit(ptr->vehicle);
        mine_vehicles.at(indice)->setVelocity(ptr->velocity);
        mine_vehicles.at(indice)->setPriority(3);

        //add Vehicle to database
        database.addVehicle(mine_vehicles.at(indice));

    }
    // otherwise, we check to see if the vehicle id already exists
    else
    {
        // call update_data
        thread t1 = std::thread(&Base_Unit::update_data, this, ptr, indice, std::ref(newVehicle));

        // have the thread join again
        t1.join();

        //now the newly added node or updated node needs to have their distance
        // checked to all the other nodes. 
        //need to check for the index again where the vehicles is.
        contains_id_number(ptr->vehicle, index);


        //this map holds vehicles that are within 50 meters or less of the input
        // vehicle.  The int (key) is the index in the master list of that vehicle
        //and the double (value) is the distance.

        map<int, double>list = checkDistancesInMasterVector1(mine_vehicles.at(index));

        //next we iterate through the map with the distances and send out alerts
        //each pair of vehicles in risk of collision will receive an alert
        if (list.size() > 0)
        {
            for (itr = list.begin(); itr != list.end(); ++itr)
            { 
                //The list contains the index in the master list of vehicles that is
               // in immediate danger of collision. Set second vehicle to this index
                Vehicle* v1 = mine_vehicles.at(itr->first);

                //this function call creates the alert messages that will be
                //sent out to each individual vehicle. We call it twice. 
                createAlert(alertMessage, alertLogMessage, fileName, ptr->velocity,
                    mine_vehicles.at(index), v1, itr->second, ptr->time);

                createAlert(alertMessage1, alertLogMessage1, fileName1, v1->getVelocity(),
                    v1, mine_vehicles.at(index), itr->second, ptr->time);

                // send the alerts
                auto len = static_cast<int>(alertMessage.length());
                p.writeToSerialPort(fileName, len + 1, h);

                len = static_cast<int>( alertMessage1.length());
                p.writeToSerialPort(fileName1, len + 1, h);

                //we also need to create the alert log message
                // remove the \n from the end of the message
                alertLogMessage = alertLogMessage.substr(0, alertLogMessage.length() - 2);

                alertLogMessage1 = alertLogMessage1.substr(0, alertLogMessage1.length() - 2);

                cout << alertLogMessage << endl;
                cout << alertLogMessage1 << endl;
                
                //get the file path and write to the alert log
                fileHandler.logToFile(alertLogMessage, MessageType::alert); 

                fileHandler.logToFile(alertLogMessage1, MessageType::alert);
            }
        }

        //update the current vehicle before leaving the function
        // if the mine_vehicle vector is larger than 1 and this is not a new
        // vehicle, then we update the database with the new message information
        if ((get_size(mine_vehicles) > 1) && (newVehicle == 0))
        {
            //if the vehicle is at risk, we need to set the flag that tracks if
            // this is the first message about being at risk from true to false
            if (mine_vehicles.at(index)->getPriorityNumber() == 0)
            {
                mine_vehicles.at(index)->setNewRisk(false);
            }
            database.updateVehicle(mine_vehicles.at(index));
        }

        // Otherwise, if this vehicle does not already exist in the database,
        // we need to add it. 
        else if ((get_size(mine_vehicles) > 1) && (newVehicle == 1))
        {
            if (mine_vehicles.at(index)->getPriorityNumber() == 0)
            {
                mine_vehicles.at(index)->setNewRisk(false);
            }
            database.addVehicle(mine_vehicles.at(index));
        }
        // need to update status of other vehicles and update them if necessary
        checkStatusAndUpdate(index);

        // now we need to sort the mine_vehicles by priority number
        std::sort(mine_vehicles.begin(), mine_vehicles.end(), Vehicle::compById);
    }
}

/*
===============
update_data

This function takes a pointer to a struct message that contains the data
 * that was contained in the most recent message.  The indice represents
 * the index of the mine_vehicles vector that the vehicle object was added to.
 * The function will check for duplicate id's. If the id is not a duplicate,
 * then the information is added to the newly created vehicle object. If it is
 * a duplicate, then the current information for that vehcile object is
 * updated.  
===============
*/
void Base_Unit::update_data(struct message* ptr, int indice, int &newVehicle)
{
    mtx_update_master.lock();
    int duplicate;
    unsigned long int index;
    //check for a duplicate id
    duplicate = contains_id_number(ptr->vehicle, index);

    //if duplicate is 0, this means that this is a new vehicle
    if (duplicate == 0)
    {
        mine_vehicles.at(indice)->setBearing(ptr->bearing);
        mine_vehicles.at(indice)->setLatitude(ptr->latitude);
        mine_vehicles.at(indice)->setLongitude(ptr->longitude);
        mine_vehicles.at(indice)->setTime(ptr->time);
        mine_vehicles.at(indice)->setUnit(ptr->vehicle);
        mine_vehicles.at(indice)->setVelocity(ptr->velocity);
        mine_vehicles.at(indice)->setPriority(3);
        newVehicle = 1;
    }
    //otherwise, this is a duplicate id and we need to update
    // the Vehicle objects location data
    else
    {
        //first we need to erase the duplicate we added to the end of the
        // vector and then just update the existing vehicle at its index
        //delete the ptr created for the vector.
        delete mine_vehicles.at(mine_vehicles.size() - 1);
        mine_vehicles.erase(mine_vehicles.begin() + mine_vehicles.size() - 1);
        setVehicleInMineVehicles2(mine_vehicles.at(index), ptr->time, ptr->latitude, ptr->longitude, ptr->velocity, ptr->bearing, -1, "active");
    }
    mtx_update_master.unlock();
}

/*
===============
get_size

returns the size of the mine_vehicles vector
===============
*/
size_t Base_Unit::get_size(const vector<Vehicle*>& v)
{
    return v.size();
}

/*
===============
contains_id_number

This function takes a vehicle id and an address to an integer that represents
 * the index of the mine_vehicles vector where the vehicle is located. It iterates
 * thru the vector, finds the index, then sets the value of index to it. If the
 * the vehicle id exists in the vector, then the function returns a 1 which means
 * true.  If it does not contain that vehicle, then a 0 is returned which means
 * false
===============
*/
int Base_Unit::contains_id_number(int id, unsigned long int& index)
{
    for (unsigned long int i = 0; i < getMineVehicles().size(); i++)
    {
        if (getMineVehicles().at(i)->getUnit() == id)
        {
            index = (i);
            return 1;
        }
    }
    return 0;
}


/*
===============
setVehicleInMineVehicles

This function updates mine_vehicles (master vector) at a specific index.  If you only
want to update one value, set the other inputs to -1 and nothing will be changed.
===============
*/
void Base_Unit::setVehicleInMineVehicles(int index, int time, double latitude, double longitude, double velocity, double bearing, int priority, string status)
{
    if (time != -1)
    {
        //set the previous time first using the current time before updating
        // the system. 
        mine_vehicles.at(index)->setTime(time);
 
    }
    if (latitude != -1)
    {
        //call to setLatitude will both update the previous_latitude as well
        //as set the new latitude
        mine_vehicles.at(index)->setLatitude(latitude);
    }
    if (longitude != -1)
    {
        //call to setLongitude will both update the previous_longitude as well
        //as set the new longtiude
        mine_vehicles.at(index)->setLongitude(longitude);
    }
    if (velocity != -1)
    {
        mine_vehicles.at(index)->setVelocity(velocity);
    }
    if (bearing != -1)
    {
        mine_vehicles.at(index)->setBearing(bearing);
    }
    if (priority != -1)
    {
        mine_vehicles.at(index)->setPriority(priority);

        //if the priority was set to 0, then we need to update the status to at-risk
        if (mine_vehicles.at(index)->getPriorityNumber() == 0)
        {
            mine_vehicles.at(index)->setStatus("at_risk");
        }
    }

    if (status != "")
    {
        //if the status had a value, then set the status to the new value
        mine_vehicles.at(index)->setStatus(status);

    }
}

/*
===============
setVehicleInMineVehicles2

This function differs from setVehicleInMineVehicles in the parameters it takes.
The first parameter is a pointer to a vehicle whereas in setVehicleInMineVehicles
the first parameter is the index in the mine_vehicles with the vehicle we
wish to update. If you only want to update one value, set the other inputs to - 1 
and nothing will be changed.
===============
*/
void Base_Unit::setVehicleInMineVehicles2(Vehicle* v, int time, double latitude,
    double longitude, double velocity, double bearing, int priority, string status)
{
    if (time != -1)
    {
        //call to setTime will both change the previous_time and set the new time
        v->setTime(time);
 
    }
    if (latitude != -1)
    {
        //call to setLatitude will both update the previous_latitude as well as
        //set the new latitude
        v->setLatitude(latitude);
    }
    if (longitude != -1)
    {
        //call to setLongitude will both update the previous_longitude as well 
        // as set the new longitude
        v->setLongitude(longitude);
    }
    if (velocity != -1)
    {
        v->setVelocity(velocity);
    }
    if (bearing != -1)
    {
        v->setBearing(bearing);
    }
    if (priority != -1)
    {
        v->setPriority(priority);

        //if the priority is changed to a 0, then we need to change the
        // status to at_risk
        if (v->getPriorityNumber() == 0)
        {

            v->setStatus("at_risk");
        }
    }

    if (status != "")
    {
        //if status is not empty, then set the value
        v->setStatus(status);

    }
}
/*
===============
checkDistancesInMasterVector1

 Function iterates through the master list and checks the distance each
 * vehicle is from the other vehicles. Tt adds any vehicles that are close to that
 * input vehicle to a vector that contains ints which represent the index number
 * in the vector of vehicles (mine_vehicles) where there is risk of collision
 //Reference for maps: https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
================
*/
map<int, double> Base_Unit::checkDistancesInMasterVector1(Vehicle* v)
{
    Calculations c;
    //this map holds the list of vehicles that will need alerts sent
    map<int, double> listOfAlerts;

    //these bools represent whether or not a priority of 0, 1, or 2 has been 
    // set in this iteration
    bool set0 = false;
    bool set1 = false;
    bool set2 = false;

    // an integer that holds a priority number
    int p;
    double distance;

    //iterate through the master list of vehicles
    for (unsigned long int i = 0; i < getMineVehicles().size(); i++)
    {

        if (getMineVehicles().at(i)->getUnit() != v->getUnit())
        {

            // get the distance
            distance = c.haversine(v, mine_vehicles.at(i));

            //distance returns as km, need to change to meters so multiply
            // by 1000
            distance = 1000 * distance;

            //update the current vehicles distances from other vehicles
            v->updateVehicleMap(v, getMineVehicles().at(i)->getUnit(), distance);

            //we also need to update the other vehicles distances as well. 

            v->updateVehicleMap(getMineVehicles().at(i), v->getUnit(), distance);

            //if the result is 50 or less than the index of the vehicle in the 
            // master list and it's distance is inserted into the map
            if (distance <= 50)
            {
                listOfAlerts.insert(pair<int, double>(i, distance));
                // set both of the vehicles priority numbers to 0
                setVehicleInMineVehicles(i, -1, -1, -1, -1, -1, 0, "at_risk");
                setVehicleInMineVehicles2(v, -1, -1, -1, -1, -1, 0, "at_risk");
                // a 0 priority was set, so this needs to remain
                set0 = true;
                //update the vehicle in the list that is at_risk
               //database.updateSingleVehicleTrait<string>("status", getMineVehicles().at(i)->getUnit(), "at-risk");
            }

            else if (distance > 50 && distance <= 75)
            {
                /*need to search the vehicle at i's list to make sure it does
                 *not have a higher priority before changing. p represents the
                 *priority of the other vehicle this loop is comparing against.*/
                p = (checkOtherVehiclesPriorityNumbers(v, i, 1));
                setVehicleInMineVehicles(i, -1, -1, -1, -1, -1, p, "");

                /* If the vehicle has not had a priority of 0 set yet, we can
                   set it to 1. */
                if (!set0)
                {
                    /*set the priority to 1*/
                    setVehicleInMineVehicles2(v, -1, -1, -1, -1, -1, 1, "");
                    set1 = true;
                }

            }

            else if (distance > 75 && distance < 100)
            {
                /* check the other vehicles distances from other vehicles before
                   we change it's overall priority number
                 */
                p = checkOtherVehiclesPriorityNumbers(v, i, 2);
                /* set the other vehicles priority number */
                setVehicleInMineVehicles(i, -1, -1, -1, -1, -1, p, "");

                /* for the current vehicle, if it has not already been set to
                 * a priority 0 or 1, then set to a priority 2 */
                if (!set0 && !set1)
                {
                    setVehicleInMineVehicles2(v, -1, -1, -1, -1, -1, 2, "");
                    set2 = true;
                }

            }

            else if (distance >= 100)
            {
                /* check the other vehicles distances from vehicles in its map
                   before we change it's overall priority number
                 */
                p = checkOtherVehiclesPriorityNumbers(v, i, 3);
                /*set the other vehicle to the lowest priority number based
                  on distances it is from other vehicles in its map*/
                setVehicleInMineVehicles(i, -1, -1, -1, -1, -1, p, "");

                /* for the current vehicle, it it has not yet been set to a
                 * priority 0, 1, or 2, then set it to a priority 3 */
                if (!set0 && !set1 && !set2)
                {
                    setVehicleInMineVehicles2(v, -1, -1, -1, -1, -1, 3, "");
                }
            }
            //update the vehicle that was compared against in the for-loop
            // database.updateVehicle(mine_vehicles.at(i));

        }
    }
    return listOfAlerts;
}

/*
===============
checkOtherVehiclesPriorityNumbers
this function checks the vehicle at the index of the mine_vehicle to see what
   the smallest distance is to any other vehicle.It returns this distance.
===============
*/
int Base_Unit::checkOtherVehiclesPriorityNumbers(Vehicle* v1, int index, int priority_number)
{
    double d;
    int priority = 3;
    
    //if the smallest distance from a vehicle is 50 or less meters, then the
    // priority value is 0
    d = v1->findSmallestDistance(mine_vehicles.at(index));
    if (d <= 50)
    {
        priority = 0;
    }

    // if the smallest distance is between 51 to 75, and the priority is not
    //0 or already 1, the set the priority to 1
    else if (d > 50 && d <= 75 && priority_number > 1)
    {
        priority = 1;
    }

    // if the smallest distance is between 76 and 100 and the priority number
    // is 3 then set the priority to 2
    else if (d > 75 && d <= 100 && priority_number > 2)
    {
        priority = 2;
    }

    // otherwise, the priority number remains unchanged. 
    else
    {
        priority = priority_number;
    }
    return priority;
}

/*
===============
checkStatusAndUpdate
This function checks to see if the status of vehicles in the database has 
changed. Status is defined by the following:
(1) active -- the vehicle is sending messages within the SECOND_LIMIT and is 
              moving.  Moving is determined by if there is any change in the 
              latitude or longitude
(2) inactive -- the vehicle is sending messages within the SECOND_LIMIT but is
              not moving.  This means that when its current and past latitude and
              longitude are compared, they are exactly the same
(3) at_risk -- This means that a vehicle is 50 meters or less away from another
              vehicle or object being tracked in the system
(4) offline -- This means that a vehicle has not sent any messages within the
              SECOND_LIMIT time frame. 

Explanation of newRisk flag: 
If the newRisk flag is true, this means that the vehicle is not currently at
risk.  When a vehicle becomes at_risk, then the flag should be false because
it is no longer new. When a vehicles status changes back from being at_risk
to another status, such as active, then the flag will be reset to true. 

The reason for the flag system is to avoid making database calls if the status
is not new.
The function will only update and call the database if there has been a change. 
===============
*/
void Base_Unit::checkStatusAndUpdate(int index)
{
    //iterate thru the mine_vehicle vector
    for (unsigned long int i = 0; i < getMineVehicles().size(); i++)
    {
        //if the unit ID of the vehicle is not the same as the vehicle that sent the most recent 
        //message, then we start checking
        if (getMineVehicles().at(i)->getUnit() != mine_vehicles.at(index)->getUnit())
        {
           
            //check if the vehicle is at_risk. If the newRisk flag is true, then
            //this means that 
            if (getMineVehicles().at(i)->getPriorityNumber() == 0 && (getMineVehicles().at(i)->getNewRisk() == true))
            {
                
                mine_vehicles.at(i)->setNewRisk(false);
     
                database.updateSingleVehicleTrait<string>("status", getMineVehicles().at(i)->getUnit(), "at_risk");
            }

            // check if the vehicle is offline or not
            //uncomment the next line of code if it is a real world scenario
            //bool check = checkOffline(getMineVehicles().at(i)->getTime());
            bool check = checkOfflineSimulate(mine_vehicles.at(index)->getTime(), getMineVehicles().at(i)->getTime());

            // if the vehicle has no recent messages and is not a priority 0, set status to offline. 
            if (check == true && (getMineVehicles().at(i)->getPriorityNumber() != 0))
            {
                if (getMineVehicles().at(i)->getStatus() != "offline")
                {
                    getMineVehicles().at(i)->setStatus("offline");
                    getMineVehicles().at(i)->setNewRisk(true);
                    database.updateSingleVehicleTrait<string>("status", getMineVehicles().at(i)->getUnit(), "offline");
                }
            }

            //if the vehicle has recent messages but has not moved and is not at risk, then
            // set to either active or inactive
            else if (check == false && (getMineVehicles().at(i)->getPriorityNumber() != 0))
            {
                // if both the latitude and longitude have not changed, then set to inactive
                if ((getMineVehicles().at(i)->getLatitude() == getMineVehicles().at(i)->getPreviousLatitude())
                    && (getMineVehicles().at(i)->getLongitude() == getMineVehicles().at(i)->getPreviousLongitude()))
                {
                    if (getMineVehicles().at(i)->getStatus() != "inactive")
                    {
                        getMineVehicles().at(i)->setStatus("inactive");
                        //reset newRisk to true because the status changed. Setting
                        // to true means that the vehicle was not just in an at_risk status. 
                        getMineVehicles().at(i)->setNewRisk(true);
                        database.updateSingleVehicleTrait<string>("status", getMineVehicles().at(i)->getUnit(), "inactive");
                    }
                }
                else
                {
                    //otherwise if the vehicle status is not currently active, then
                    // set to active because this means that the vehicle moved. 
                    if (getMineVehicles().at(i)->getStatus() != "active")
                    {
                        getMineVehicles().at(i)->setStatus("active");
                        getMineVehicles().at(i)->setNewRisk(true);
                        database.updateSingleVehicleTrait<string>("status", getMineVehicles().at(i)->getUnit(), "active");

                    }
                }
            }
        }
    }
}

/*
===============
logToFile
This function allows the base unit to call a fileHandler.
===============
*/
void Base_Unit::logToFile(std::string message, MessageType type)
{
    fileHandler.logToFile(message, type);
}