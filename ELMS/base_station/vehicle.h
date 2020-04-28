/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

//#include <queue>
#include <vector>

#ifndef VEHICLE_HPP
#define VEHICLE_HPP

//using std::priority_queue;
#include <iostream>
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

        //create getter functions
        int getUnit();
        int getTime();
        double getLatitude();
        double getLongitude();
        double getVelocity();
        double getBearing();
        vector<Vehicle> getVehicleVector();

        //regular functions

        //Sort the vehicle vector
        void sortVehicleVector();
        //add a vehicle to the vector
        void addVehicleVector(Vehicle, int);
        //check if a vehicle is in the vector
        bool checkVehicleVector(int);
        
};

#endif
