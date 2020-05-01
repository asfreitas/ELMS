#pragma once

#include <string>
#include <iostream>
#include "vehicle.h"
#include <algorithm>
#include <sstream>
#include <functional>
#include <map>
#include <set>
using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::ostringstream;
using std::make_pair;

int stringToInt(string str);

double stringToDouble(string str);
char* stringToCharPointer(string& str, char* name);
string doubleToString(double num);
bool sortByVal(const pair<int, double>& a, const pair<int, double>& b);
vector<pair<int, double>>* sortMap(map<int, double>&);
void printMap(map<int, double>* mapVehicles);

/*This struct is used in the Base_Unit class to delete the pointers that are
 * stored in the mine_vehicles vector. The reference were I found this is below/
/*https://stackoverflow.com/questions/891913/c-stdvector-of-pointers-deletion-and-segmentation-faults#891924
*/
struct deleteVector
{
    template <class T>
    void operator()(T* ptr)const
    {
        delete ptr;
    }
};

