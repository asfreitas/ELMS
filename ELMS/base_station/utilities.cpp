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