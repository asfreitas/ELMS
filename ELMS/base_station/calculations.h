/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#define _USE_MATH_DEFINES
#include <cmath>
#include <iostream>
#include <string>
#include "utilities.h"
#include "vehicle.h"


const double EARTH_RADIUS = 6371;

#ifndef CALCULATIONS_H
#define CALCULATIONS_H

class Calculations
{
    private:



    public:
        double degrees_to_radians(double degreeMinutes);
        double radians_to_degrees(double radians);
        double knots_to_mps(int knots);
        double haversine(Vehicle* vehicle1, Vehicle* vehicle2);
        void updateVehicle(Vehicle* vehicle1, Vehicle* vehicle2);
        int getBearing(Vehicle* vehicle1, Vehicle* vehicle2);
        int normalizeBearing(double);
        

};


#endif