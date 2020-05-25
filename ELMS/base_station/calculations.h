/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

#ifndef CALCULATIONS_H
#define CALCULATIONS_H

#define _USE_MATH_DEFINES

#include "utilities.h"
#include "vehicle.h"

#include <cmath>
#include <iostream>
#include <string>



const double EARTH_RADIUS = 6371;



class Calculations
{
    private:

    public:
        double degrees_to_radians(double degreeMinutes);
        double radians_to_degrees(double radians);
        double knots_to_mps(double knots);
        double haversine(Vehicle* vehicle1, Vehicle* vehicle2);
        int getBearing(Vehicle* vehicle1, Vehicle* vehicle2);
        int normalizeBearing(double);
        

};
#endif