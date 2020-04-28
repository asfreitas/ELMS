/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

#include "calculations.h"

/*
=============
knots_to_mps
=============
*/
double Calculations::knots_to_mps(int knots){

	double mps = -1;

	mps = (knots/1.94384);

	return mps;

}

/*
=============
degrees_to_radians
=============
*/
double Calculations::degrees_to_radians(double degreeMinutes){

	int degrees;
	double minutes, decimal, degreesDecimal, radians;
	
	degrees = (int)degreeMinutes / 100; // use loss of precision to get degrees
	minutes = degreeMinutes - ((double)degrees * 100);
	decimal = minutes/60;
	degreesDecimal = degrees + decimal;
	radians = degreesDecimal * (M_PI/180);



	return radians;
}

/*
=============
radians_to_degrees
=============
*/
double Calculations::radians_to_degrees(double radians)
{
	return radians * (180 / M_PI);
}

/*
=============
getBearing

//https://www.movable-type.co.uk/scripts/latlong.html
=============
*/
int Calculations::getBearing(Vehicle* vehicle1, Vehicle* vehicle2)
{
	double longDelta, x, y, bearing;

	longDelta = vehicle2->getLongitude() - vehicle1->getLongitude();
	y = std::sin(longDelta) * std::cos(vehicle2->getLatitude());
	x = std::cos(vehicle1->getLatitude()) * std::sin(vehicle2->getLatitude())
		- std::sin(vehicle1->getLatitude()) * std::cos(vehicle2->getLatitude()) * std::cos(longDelta);
	bearing = radians_to_degrees(std::atan2(y, 2));
	
	return normalizeBearing(bearing);
}



/*
=============
haversine

//https://community.esri.com/groups/coordinate-reference-systems/blog/2017/10/05/haversine-formula
=============
*/
double Calculations::haversine(Vehicle *vehicle1, Vehicle *vehicle2){

	double a, result, phi_1, 
		phi_2, delta_phi, delta_lambda;
	
	phi_1 = degrees_to_radians(vehicle1->getLatitude());
	phi_2 = degrees_to_radians(vehicle2->getLatitude());
	double lambda_1 = degrees_to_radians(vehicle1->getLongitude());
	double lambda_2 = degrees_to_radians(vehicle2->getLongitude());
	delta_phi = phi_2 - phi_1;
	delta_lambda = lambda_2 - lambda_1;

	a = std::sin(delta_phi / 2.0) * std::sin(delta_phi / 2.0)
		+ std::cos(phi_1) * std::cos(phi_2) * std::sin(delta_lambda / 2.0) * std::sin(delta_lambda / 2.0);

	result = 2 * std::atan2(std::sqrt(a), std::sqrt(1-a));



	return result * EARTH_RADIUS;
}

/*
=============
normalizeBearing
=============
*/
int Calculations::normalizeBearing(double bearing)
{
	int newBearing = (int)bearing;
	newBearing = (newBearing + 360) % 360;
	return newBearing;
}
