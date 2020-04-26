/*
 * ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

#include "calculations.h"


double Calculations::knots_to_mps(int knots) {

	double mps = -1;

	mps = (knots / 1.94384);

	return mps;

}


double Calculations::degrees_to_radians(double degreeMinutes) {

	int degrees;
	double minutes, decimal, degreesDecimal, radians;

	degrees = degreeMinutes / 100; // use loss of precision to get degrees
	minutes = degreeMinutes - ((double)degrees * 100);
	decimal = minutes / 60;
	degreesDecimal = degrees + decimal;
	radians = degreesDecimal * (M_PI / 180);

	return radians;

}

//https://community.esri.com/groups/coordinate-reference-systems/blog/2017/10/05/haversine-formula
//haversine calculation
double Calculations::haversine(Vehicle* vehicle1, Vehicle* vehicle2) {

	double distance, result, v1long, v1lat, v2long, v2lat, delta_phi, delta_lambda;

	v1long = degrees_to_radians(vehicle1->getLongitude());
	v1lat = degrees_to_radians(vehicle1->getLatitude());
	v2long = degrees_to_radians(vehicle2->getLongitude()); // set variables
	v2lat = degrees_to_radians(vehicle2->getLatitude());

	delta_phi = v2lat - v1lat;
	delta_lambda = v2long - v1long;

	distance = std::asin(std::sqrt(std::pow(std::sin(delta_phi / 2), 2)
		+ std::cos(v1lat) * std::cos(v2lat) * std::pow(std::sin(delta_lambda / 2), 2)));

	result = 2 * EARTH_RADIUS * distance;



	return result;
}

void Calculations::updateVehicle(Vehicle* vehicle1, Vehicle* vehicle2)
{
}
