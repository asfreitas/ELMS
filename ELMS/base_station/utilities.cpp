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
