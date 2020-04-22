/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar 
*/

#include <iostream>
#include <cstdio>
#include <vector>
#include <sstream>
#include <algorithm>
#include "utilities.h"

using std::stringstream;
using std::cout;
using std::endl;
using std::cerr;
using std::stoi;
using std::stof;
using std::vector;
using std::replace;

struct message {
	int vehicle;
	int time;
	double latitude;
	double longitude;
	int velocity;
	int bearing;

};


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



/*
int main(){

	//opens error_log.txt and points stderr to point to it when printing
	freopen("error_log.txt", "w", stderr);

	string testMessage = "$1, 125, 12321.213, 123.231, 2, 12*";

	const char* messageFile = testMessage.c_str();
	const char* fileName = "received_message.txt";

	string fakeMessage = "34, 3921, 12384.133, 1234.233, 123.32, 1234";

	int unit = -1, messageTime = -1;
	double latitude = -1, longitude = -1, velocity = -1, bearing = -1;
	
	//string to parse message;
	vector <string>token;
	string temp;
	char delim = ',';
	
	//check that first character is $
	if(testMessage[0] == '$' && testMessage.back() == '*'){

		//first, store message into filestoreMessage(fileName, testMessage);
		storeMessage(fileName, messageFile);
		//remove spaces
		replace(testMessage.begin(), testMessage.end(), '*', '\0');
	
		stringstream check1(testMessage);
	
		while(getline(check1, temp, delim)){
			//set parsed string
			token.push_back(temp);;
		}

		for(int i = 0; i < token.size(); i++){
			token[i].erase(0, 1);
			cout << token[i] << endl;
			if(i == 0){
				unit = stringToInt(token[i]);
				printf("%d\n", unit);
			}else if(i == 1){
				messageTime = stringToInt(token[i]);
				printf("%d\n", messageTime);
			}else if(i == 2){
				latitude = stringToDouble(token[i]);
				printf("%.4f\n", latitude);
			}else if(i == 3){
				longitude = stringToDouble(token[i]);	
				printf("%.4f\n", longitude);
			}else if(i == 4){
				velocity = stringToDouble(token[i]);
				printf("%.2f\n", velocity);
			}else if(i == 5){
				bearing = stringToDouble(token[i]);
				printf("%.2f\n", bearing);
			}
		}
	
	}else{
		cout << "String not formatted correctly" << endl;
		cerr << "Incoming string not formatted correctly" << endl;
	}


	//store message info into vehicle object
	

	return 0;
}
*/