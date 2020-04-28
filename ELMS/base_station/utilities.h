#pragma once

#include <string>
#include <iostream>
#include "vehicle.h"
#include <algorithm>
using std::string;
using std::cout;
using std::endl;
using std::exception;

int stringToInt(string str);

double stringToDouble(string str);
char* stringToCharPointer(string& str, char* name);