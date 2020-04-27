/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar 
*/

#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <algorithm>
#include "utilities.h"
#include "parse_incoming.h"

using std::stringstream;
using std::cout;
using std::endl;
using std::cerr;
using std::stoi;
using std::stof;
using std::vector;
using std::replace;



void storeMessage(const char* fileName, const char* message){
	//FILE *inputFile;
	//inputFile = fopen(fileName, "a");

	//fprintf(inputFile, "%s\n", message);	

	//fclose(inputFile);

}

message* createNewMessage(string incomingMessage)
{
	message* newMessage = new message; // create new message

	string temp;	// setup tokenizer
	char delim = ',';

	incomingMessage.erase(remove_if(incomingMessage.begin(), incomingMessage.end(), isspace), incomingMessage.end());

	incomingMessage = incomingMessage.substr(1, incomingMessage.length() - 1); // remove $ and *

	stringstream check1(incomingMessage);

	
	for (int dataPoint = 0; dataPoint < 6; dataPoint++)
	{
		getline(check1, temp, delim);

		switch (dataPoint)
		{
		case 0: newMessage->vehicle = stringToInt(temp); break;
		case 1: newMessage->time = stringToInt(temp); break;
		case 2: newMessage->latitude = stringToDouble(temp); break;
		case 3: newMessage->longitude = stringToDouble(temp); break;
		case 4: newMessage->velocity = stringToInt(temp); break;
		case 5: newMessage->bearing = stringToInt(temp); break;
		default: break;
		}
	}
	return newMessage;

}