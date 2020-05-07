#pragma once

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <string>
#include <iostream>

#include "vehicle.h"


//ref https://github.com/mongodb/mongo-cxx-driver/blob/master/examples/mongocxx/instance_management.cpp


class Database {
	typedef mongocxx::pool::entry connection;
	typedef mongocxx::instance instance;
	typedef mongocxx::database database;
	typedef mongocxx::collection collection;

private:
	mongocxx::uri uri;
	mongocxx::pool *pool;

public:
	Database(std::string);
	~Database();
	connection getConnection();
	void updateVehicle(Vehicle*);





};

