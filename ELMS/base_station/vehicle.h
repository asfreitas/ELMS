/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

//#include <queue>
#include <vector>

#ifndef VEHICLE_HPP
#define VEHICLE_HPP

//using std::priority_queue;
#include <iostream>
#include <algorithm>
#include <map>
#include <iterator>
using std::cout;
using std::endl;
using std::vector;
using std::map;
using std::pair;
using std::iterator;

class Vehicle
{
    private:
        int unit;
        int time;
        double latitude;
        double longitude;
        double velocity;
        double bearing;
        int priority;
        vector<Vehicle> vectorVehicles;
        // this map holds the unit number of another vehicle and the distance
        // it is away from this vehicle object
        map<int, double>distance_to_other_vehicles;

    public:     
        //create default constructor
        Vehicle();

        //Create constructor
        Vehicle(int unit, int time, double latitude, double longitude, double velocity, double bearing, int priority);
        //create destructor
        ~Vehicle();

        //create setter functions
        void setUnit(int);
        void setTime(int);
        void setLatitude(double);
        void setLongitude(double);
        void setVelocity(double);
        void setBearing(double);
        void setPriority(int);

        //create getter functions
        int getUnit();
        int getTime();
        double getLatitude();
        double getLongitude();
        double getVelocity();
        double getBearing();
        int getPriorityNumber();
        map<int, double>* getMapOfVehicles();
        int getMapSize(Vehicle& v);
        int getVehicleID() { return unit; }

        //regular functions

        //Sort a vector of Vehicles by priority number
        void sortVehicleVector1(vector<Vehicle*>);
        //add a vehicle to the vector
        void addVehicleVector(Vehicle, int);
        //check if a vehicle is in the vector
        bool checkVehicleVector(int);
        void updateVehicleMap(Vehicle &v, int vehicle_id, double distance);
        void updateVehicleMap1(Vehicle* v, int vehicle_id, double distance);
        /* https://stackoverflow.com/questions/16366978/stdsort-on-a-vector-of-class-pointers*/
        static bool compById(const Vehicle* a, const Vehicle* b)
        {
            return a->priority < b->priority;
        }

        
};

#endif
