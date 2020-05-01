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

