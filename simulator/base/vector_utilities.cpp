#include "vector_utilities.hpp"
using std::cout;
using std::endl;
using std::setprecision;

void input_data(vector<Vehicle> &v, int id, int time, double lat, double lon, int vel, int bear)
{
	int size = -1;
	int duplicate;
	Vehicle vehicle;
	
	size = get_size(v);
	//if the size of the vector is 0, then we know we need to add the 
	// vehicle to the vector
	if(size == 0)
	{
		/* we create a Vehicle object and add it to our vector mine_vehicles */
		vehicle = Vehicle(id, time, lat, lon, vel, bear);
		v.push_back(vehicle);
	}
	// otherwise, we check to see if the vehicle id already exists
	else
	{
		duplicate = contains_id_number(v, id);
		//if the id number is not a duplicate, then create a new
		//vehicle object and add it to the vector.
		if(duplicate == 0)
		{
			vehicle = Vehicle(id, time, lat, lon, vel, bear);
			v.push_back(vehicle);
		}
		//otherwise, this is a duplicate id and we need to update
		// the Vehicle objects location data
		else
		{
			//To do: update Vehice objects data
			std::cout << "Duplicate id number: " << id  << std::endl;
			
		}
	 }
	 	 
}
// returns vector size
int get_size(vector<Vehicle> &v)
{
	return v.size();
}

//check to see if vector contains a specific id number
int contains_id_number(vector<Vehicle> &v, int id)
{
	for(auto itr : v)
	{
		if(itr.get_id() == id)
		{
			return 1;
		}
	}
	return 0;	
}

/* prints the contents held in the Vector*/
void print_vector(vector<Vehicle> &v)
{
	cout << "**********************************" << endl << endl;
		
		for(auto itr: v)
		{
			cout << "*****VEHICLE " << itr.get_id() << " *****" << endl;
			cout << std::fixed;
			cout << std::setprecision(4);
			cout << "latitude: "<<itr.get_latitude() <<endl;
			cout << "longitude: "<< itr.get_longitude() <<endl;
			cout << std::fixed;
			cout << std::setprecision(0);
			cout << "time_stamp: "<< itr.get_time_stamp() <<endl;
			cout << "velocity: " << itr.get_velocity() <<endl;
			cout << "bearing: " <<itr.get_bearing() <<endl << endl;
		}
	cout << "***************************************" << endl << endl;
				
}
