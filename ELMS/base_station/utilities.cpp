/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#include "utilities.h"



/* Function converts a string into an integer*/
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

/* Function takes a string and returns a double*/
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
/* Function takes a string and returns a pointer to a char array*/
/*Reference https://www.geeksforgeeks.org/convert-string-char-array-cpp*/
char* stringToCharPointer(string& str, char* fileName)
{
	memset(fileName, '\0', 100);
	for (size_t i = 0; i < str.length() + 1; i++)
	{
		fileName[i] = str[i];
	}
	return fileName;
}
/* Function takes a double and returns a string*/
/* Reference: https://stackoverflow.com/questions/332111/how-do-i-convert-a-double-into-a-string-in-c*/
string doubleToString(double num)
{
	ostringstream str1;
	str1 << num;
	string str2 = str1.str();
	return str2;

}
/* Reference for this is: https://thispointer.com/how-to-sort-a-map-by-value-in-c/
   What this funtion does is take the address of a vector pair and map. It then 
   copies the key/values from the map into the vector of pairs, then sorts these
   pairs and returns the sorted vector of pairs.  The vector of pairs is
   sorted based on the second element which is distance.
*/
vector<pair<int,double>> * sortMap(vector<pair<int, double>>& vehicles, map<int, double>& mapOfDistances)
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

/* This is a comparator function that is used in a sort for a map.*/
// Reference: https://www.educative.io/edpresso/how-to-sort-a-map-by-value-in-cpp
bool sortByVal(const pair<int, double>& a, const pair<int, double>& b)
{
	return (a.second < b.second);
}

/* This function takes a pointer to a map and prints it's contents 
   * Reference: https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
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
===============================================================================
secondsBetweenTime
This function takes 3 arguments.  The first is a 6 digit time in Zulu that 
represents the most current time.  The second is a 6 digit time in Zulu that
represents the previous time.  The 3rd represents the number of seconds to allow
between the time intervals up to 1 hour or 3600 seconds.  
If the difference between the current time and the previous time is more than
the numSecondsToCheckFor then a 1 is returned for true.  If it is less, than 
a 0 is returned for false. If the numSecondsToCheckFor is greater than 3600, than
a -1 is returned for being invalid. 
*/
int secondsBetweenTime(int value1, int value2, int numSecondsToCheckFor)
{
	if (numSecondsToCheckFor <= 60)
	{
		//we are only check for 60 seconds or less so we only need to look at
		// the last two digits of the 6 digit number.  We can get the last 2
		// digits by using modulo 100.
		int val1 = value1 % 100;
		int val2 = value2 % 100;

		//subtract the two values. 
		int numSeconds = val1 - val2;
		
		//if it is a negative value, we add 60 seconds. 
		if (numSeconds < 0)
		{
			numSeconds += 60;
		}
		
		// if the numSeconds is greater than the amount we were allowing, then
		// we return true. 
		if (numSeconds > numSecondsToCheckFor)
		{
			return 1;
		}
		// otherwise, we return false
		else
		{
			return 0;
		}
	}
	// if we are checking for more than 60 seconds but less than or equal to 
	// 1 hour or 3600 seconds, then we do a similiar thing to above. 
	else if (numSecondsToCheckFor > 60 && numSecondsToCheckFor <= 3600)
	{
		//if we add 60 seconds, we will need to subtract 1 from the minutes. 
		int flag = 0;
		int val1 = value1 % 100;
		
		int val2 = value2 % 100;
		int numSeconds = val1 - val2;
		
		if (numSeconds < 0)
		{
			numSeconds += 60;
			//set the flag to true so that we add 1 minute to the second time
			flag = 1;
		}
		//get the middle two digitis of the 6 digit number that represent the
		// minutes by using modulo and then division
		int val3 = (value1 % 10000)/100;
		int val4 = (value2 % 10000)/100;
		//if the flag was set to true, then we add 1 minute to the second number
		if (flag)
		{
			//if the second number was 59, we change it to 0
			if (val4 == 59)
			{
				val4 = 0;
			}
			else {
				val4++;
			}
		}
		//check the number of minutes between the two middle numbers
		int numMinutes = val3 - val4;
		//if it is a negative number, then we add 60 seconds. 
		if (numMinutes < 0)
		{
			numMinutes += 60;
		}
		// add up the total number of seconds and compare it to the desired
		// limit of seconds. 
		int numTotal = numSeconds + (numMinutes * 60);
		if (numTotal > numSecondsToCheckFor)
		{
			return 1;
		}
		else
		{
			return 0;
		}
	}
	// return a -1, if the user entered a number larger than 3600. 
	else
	{
		return -1;
	}
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

