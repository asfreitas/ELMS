
/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#include "utilities.h"
#include <time.h>


/*
===============
stringToDouble
Function converts a string into an integer
===============
*/
int stringToInt(string str) {
	int integer = -1;

	try {
		integer = stoi(str);
	}
	catch (exception& ex) {
		cout << "ERROR: " << ex.what() << endl;
	}

	return integer;
}

/*
===============
stringToDouble

Function takes a string and returns a double
===============
*/
double stringToDouble(string str) {
	double number = -1;
	//size_t sz;
	try {
		number = stof(str);
	}
	catch (exception& ex) {
		cout << "ERROR: " << ex.what() << endl;
	}

	return number;
}
/*
===============
stringToCharPointer
Function takes a string and returns a pointer to a char array
Reference https://www.geeksforgeeks.org/convert-string-char-array-cpp
===============
*/
char* stringToCharPointer(string& str, char* fileName)
{
	memset(fileName, '\0', 100);
	for (size_t i = 0; i < str.length() + 1; i++)
	{
		fileName[i] = str[i];
	}
	return fileName;
}
/*
===============
doubleToString
Function takes a double and returns a string
Reference: https://stackoverflow.com/questions/332111/how-do-i-convert-a-double-into-a-string-in-c
===============
*/
string doubleToString(double num)
{
	ostringstream str1;
	str1 << num;
	string str2 = str1.str();
	return str2;

}
/*
===============
sortMap
Reference for this is: https://thispointer.com/how-to-sort-a-map-by-value-in-c/
   What this funtion does is take the address of a vector pair and map. It then
   copies the key/values from the map into the vector of pairs, then sorts these
   pairs and returns the sorted vector of pairs.  The vector of pairs is
   sorted based on the second element which is distance.
===============
*/
vector<pair<int, double>>* sortMap(vector<pair<int, double>>& vehicles, map<int, double>& mapOfDistances)
{

	// copy key-value pairs from the map to the vector
	for (pair<int, double> element : mapOfDistances)
	{
		vehicles.push_back(std::make_pair(element.first, element.second));
	}
	// sort the vehicles by increasing order of the distance (the second item)
	sort(vehicles.begin(), vehicles.end(), sortByVal);

	return &vehicles;
}

/*
===============
sortByVal
This is a comparator function that is used in a sort for a map.
Reference: https://www.educative.io/edpresso/how-to-sort-a-map-by-value-in-cpp
===============
*/
bool sortByVal(const pair<int, double>& a, const pair<int, double>& b)
{
	return (a.second < b.second);
}

/*
===============
printMap
This function takes a pointer to a map and prints it's contents
   * Reference: https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
===============
*/
void printMap(map<int, double>* mapVehicles)
{
	cout << "Other Vehicles and Their Distance" << endl;
	cout << "\tUnit\tDistance\n";
	for (pair<int, double> element : *mapVehicles)
	{
		cout << '\t' << element.first
			<< '\t' << element.second << '\n';
	}
	cout << endl;
}

/*
===============
roundToFourDecimals
Takes a double and rounds it to four decimal places
Reference: https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/
===============
*/
double roundToFourDecimals(double value)
{
	double temp;
	if (value < 0)
	{
		temp = (int)(value * 10000 - 0.5);
	}
	else
	{
		temp = (int)(value * 10000 + 0.5);
	}
	temp = (double)temp / 10000;
	return temp;
}

/*
===============
roundToOneDecimal
Takes a double and rounds it to one decimal place
Reference: https://www.geeksforgeeks.org/rounding-floating-point-number-two-decimal-places-c-c/
===============
*/
double roundToOneDecimal(double value)
{
	double temp;
	if (value < 0)
	{
		temp = (int)(value * 10 - 0.5);
	}
	else
	{
		temp = (int)(value * 10 + 0.5);
	}
	temp = (double)temp / 10;
	return temp;

}

/*
===============
zuluToDate
This function takes a pointer to a Vehicle and converts the zulu time
   * to a date with today's date and the zulu time and sends as time_t in seconds
===============
*/
time_t zuluToDate(int messageTime) {
	//convert time to date and time
	int hours, mins, secs = -1;

	hours = messageTime / 10000;
	mins = (messageTime - (hours * 10000)) / 100;
	secs = (messageTime - (hours * 10000) - (mins * 100));
	if (hours >= 24 || mins >= 60 || secs >= 60) {
		cout << "Invalid time" << endl;
	}

	time_t now = time(0);
	struct tm local;
	//set current date
	localtime_s(&local, &now);
	//reset time to message time
	local.tm_hour = hours;
	local.tm_min = mins;
	local.tm_sec = secs;

	//format mesasge date and time as string if needed
	//char buf[80];
	//strftime(buf, sizeof(buf), "%Y-%m-%dT%XZ", &local);

	//convert into time in seconds
	time_t messageDateTime = mktime(&local);
	messageDateTime = messageDateTime - 25200;
	return messageDateTime;
}

/*
===============
checkOffline
//checks current message time in vehicle and compares to time now. If
//within last 10 seconds, then set vehicle status to "offline"
===============
*/
bool checkOffline(int currentMessageTime) {
	time_t now = time(0) - 25200;
	//get current message time
	time_t currentTime = zuluToDate(currentMessageTime);
	if ((now - currentTime) > SECONDS_LIMIT) {
		return true;
	}
	else {
		return false;
	}
}
/*
===============
checkOfflineSimulate
 * checks the message time of the last message received from a vehicle compared
 * to the message time of the most current time received from any vehicle. 
 * This is used for testing purposes to allow script use. If
 * If the calculated difference is greater than the SECONDS_LIMIT, then true
 * is returned meaning that the vehicle is offline.  Otherwise, if it is
 * within the SECONDS_LIMIT, it is not offline.
 ===============
 */
bool checkOfflineSimulate(int currentMessageTime, int lastMessageReceived) {
	//cout << endl << endl << "Here is the currentMessageTime sent in: " << currentMessageTime << endl;
	//convert Zulu times to date time in seconds for comparison
	time_t latestMessageTime = zuluToDate(currentMessageTime);
	time_t lastMessageTime = zuluToDate(lastMessageReceived);

	//cout << "Here is now and currentTime: " << latestMessageTime << " " << lastMessageTime << endl << endl;
	if ((latestMessageTime - lastMessageTime) > SECONDS_LIMIT) {
		return true;
	}
	else {
		return false;
	}
}


