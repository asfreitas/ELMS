#include <cmath>
#include <cstdio>
#include <string>

double knots_to_mps(int knots){

	double mps = -1;

	mps = (knots/1.94384);

	return mps;

}


double degrees_to_radians(double degreeMinutes){

	int degrees = degreeMinutes / 100;

	printf("degree is %d\n", degrees);	

	double minutes = degreeMinutes - (degrees * 100);

	printf("minutes are %.4f\n", minutes);

	double decimal = minutes/60;

	printf("decimal is %.4f\n", decimal);

	double degreesDecimal = degrees + decimal;

	printf("Pi is %f\n", M_PI);

	double radians = degreesDecimal * (M_PI/180);

	printf("Radians is %.4f\n", radians);
	
}


//haversine calculation
double haversine(double latitude1, double longitude1, double latitude2, double longitude2){

	
	
}


int main(){

	int testKnots = 2;

	double mps = knots_to_mps(testKnots);

	printf("MPS is: %f\n", mps);

	double testDegreeMinutes = 7518.1234;

	degrees_to_radians(testDegreeMinutes);

	return 0;

}
