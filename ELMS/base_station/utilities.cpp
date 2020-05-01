/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/
#include "utilities.h"




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

// comparator function
// Reference: https://www.educative.io/edpresso/how-to-sort-a-map-by-value-in-cpp
bool sortByVal(const pair<int, double>& a, const pair<int, double>& b)
{
	return (a.second < b.second);
}

/* prints the key and values in a map
   Reference: https://www.geeksforgeeks.org/map-associative-containers-the-c-standard-template-library-stl/
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

