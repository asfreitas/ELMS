/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
 *
 * This file contains the message structure and function
 * header for creating new messages from that.
*/


#ifndef PARSE_INCOMING_H
#define PARSE_INCOMING_H

#include "utilities.h"

#include <string>
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

message* createNewMessage(string incomingMessage);

#endif