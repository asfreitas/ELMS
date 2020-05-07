#pragma once

#include "database.h"


Database::Database(std::string _uri)
{
	static instance instance{};
	uri = mongocxx::uri(_uri);
	pool = new mongocxx::pool{ uri };
}

Database::~Database()
{
	std::cout << "This can't go out of scope until the" 
		"program closes otherwise we lose the connection";
	delete pool;
}

Database::connection Database::getConnection()
{
	return pool->acquire();
}

void Database::updateVehicle(Vehicle* vehicle)
{

	auto connection = getConnection();

	collection vehicles = connection->database("test").collection("Vehicles");

	std::cout << vehicles.find({});

}

void addVehicle(Vehicle* vehicle)
{
	//auto connection = mongo_access::instance().get_connection();

	//connection->database("test").collection("vehicles").find({});
}
