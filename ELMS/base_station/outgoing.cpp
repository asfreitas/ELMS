#include <string>
#include <cstdio>


//and store in alert file
void outgoing_message(const char* fileName, int unit1, int unit2, int messageTime, int velocity, int distance, int bearing){

	FILE *alertFile;
	alertFile = fopen(fileName, "a");

	fprintf(alertFile, "$%d->%d, %d, %d, %d, %d*\n", unit1, unit2, messageTime, velocity, distance, bearing);

	fclose(alertFile);
}

int main(){


	//test data
	const char* fileName = "alerts.txt";
	int testUnit1 = 12;
	int testUnit2 = 67;
	int messageTime = 12302;
	int velocity = 12;
	int distance = 27;
	int bearing = 1;

	outgoing_message(fileName, testUnit1, testUnit2, messageTime, velocity, distance, bearing);

	return 0;
}
