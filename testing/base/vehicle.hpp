#ifndef VEHICLE_HPP
#define VEHICLE_HPP
#include <vector>
using std::vector;

/* create a class called Vehicle*/
class Vehicle
{
    //Access specifier
    private:

    /* Data Members*/
        int unit_id;
        double latitude;
        double longitude;
        int time_stamp;
        int velocity;
        int bearing;
		
	/* public functions */	
	public:
		Vehicle();
		Vehicle(int id, int time, double lat, double height, int vel, int heading);
		~Vehicle();
        int get_id();
        double get_latitude();
        double get_longitude();
        int get_time_stamp();
        int get_velocity();
        int get_bearing();
        void set_latitude(double lat);
        void set_longitude(double height);
        void set_time(int time);
        void set_velocity(int vel);
        void set_bearing(int heading);
        void set_id(int id);
		
		
};
#endif