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
using std::cout;
using std::endl;
using std::vector;

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

    public:

        // declare a bool operator so that we can sort any vector of vehicles
        // by priority number. Reference:
        // https://stackoverflow.com/questions/16507717/sorting-a-vector-in-c-by-accessing-a-private-member
        bool operator<(const Vehicle& other)const
        {
            return priority < other.priority;
        }
        
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
        vector<Vehicle> getVehicleVector();

        //regular functions

        //Sort a vector of Vehicles by priority number
        void sortVehicleVector(vector<Vehicle>&);
        //add a vehicle to the vector
        void addVehicleVector(Vehicle, int);
        //check if a vehicle is in the vector
        bool checkVehicleVector(int);
        
};

#endif
