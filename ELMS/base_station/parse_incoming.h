#ifndef PARSE_INCOMING_H
#define PARSE_INCOMING_H

#include <fstream>
#include <string>
#include <vector>
#include "utilities.h"
using std::vector;
using std::ofstream;
using std::string;

struct message {
	int vehicle;
	int time;
	double latitude;
	double longitude;
	int velocity;
	int bearing;
};

void storeMessage(string& fileName, string& message);
//int stringToInt(string str);
message* createNewMessage(string incomingMessage);

#endif