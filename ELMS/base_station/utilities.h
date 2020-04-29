#pragma once

#include <string>
#include <iostream>
#include "vehicle.h"
#include <algorithm>
#include <sstream>
using std::string;
using std::cout;
using std::endl;
using std::exception;
using std::ostringstream;

int stringToInt(string str);

double stringToDouble(string str);
char* stringToCharPointer(string& str, char* name);
string doubleToString(double num);