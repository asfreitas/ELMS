#ifndef VECTOR_UTILITIES_HPP
#define VECTOR_UTILITIES_HPP

#include <iostream>
#include <iomanip>
#include <cstring>
#include "vehicle.hpp"
#include <vector>

//checks if unit_id in vector yet and if not creates class object and puts
// in vector.  Does not update new values
void input_data(vector<Vehicle> &v, int id, int time, double lat, double lon, int vel, int bear);
int get_size(vector<Vehicle>& v); // returns size of vector
int contains_id_number(vector<Vehicle>& v, int id); // checks if vector has object with a specific id number
void print_vector(vector<Vehicle>& v); // prints the current contents of the vector

#endif