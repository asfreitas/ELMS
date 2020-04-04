#include <iostream>
#include "vehicle.hpp"
/* class definitions with getters and setters */
Vehicle::Vehicle()
{
    unit_id = 0;
    latitude = 0.0;
    longitude = 0.0;
    time_stamp = 0;
    velocity = 0;
    bearing = 0;
}

Vehicle::Vehicle(int id, int time, double lat, double height , int vel, int heading)
{
    unit_id = id;
    latitude = lat;
    longitude = height;
    time_stamp = time;
    velocity = vel;
    bearing = heading;
}

Vehicle::~Vehicle()
{
	
	//std::cout << "Object is being deleted" << std::endl;
	
}

int Vehicle::get_id()
{
    return unit_id;
}

double Vehicle::get_latitude()
{
    return latitude;
}

double Vehicle::get_longitude()
{
    return longitude;
}

int Vehicle::get_time_stamp()
{
    return time_stamp;
}

int Vehicle::get_velocity()
{
    return velocity;
}

int Vehicle::get_bearing()
{
    return bearing;
}

void Vehicle::set_latitude(double lat)
{
    latitude = lat;
}

void Vehicle::set_longitude(double height)
{
    longitude = height;
}

void Vehicle::set_time(int time)
{
    time_stamp = time;
}

void Vehicle::set_velocity(int vel)
{
    velocity = vel;
}

void Vehicle::set_bearing(int heading)
{
    bearing = heading;
}
void Vehicle::set_id(int id)
{
    unit_id = id;
}


		



