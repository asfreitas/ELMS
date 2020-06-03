/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

#pragma once

//#include "vehicle.h"
#define SECONDS_LIMIT 60

#include <string>
#include <iostream>
#include <algorithm>
#include <sstream>
#include <functional>
#include <map>
#include <vector>
#include <iomanip>

using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::ostringstream;
using std::make_pair;
using std::map;
using std::pair;
using std::vector;


int stringToInt(string str);

double stringToDouble(string str);
char* stringToCharPointer(string& str, char* name);
string doubleToString(double num);
bool sortByVal(const pair<int, double>& a, const pair<int, double>& b);
vector<pair<int, double>>* sortMap(map<int, double>&);
void printMap(map<int, double>* mapVehicles);

double roundToFourDecimals(double value);
double roundToOneDecimal(double value);

time_t zuluToDate(int time);
bool checkOffline(int time);
bool checkOfflineSimulate(int currentMessageTime, int lastMessageReceived);
size_t countCommas(string str);
std::string timePadding(int timeUnit);

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
