#pragma once


#ifndef DATABASE_H
#define DATABASE_H


//ref https://github.com/mongodb/mongo-cxx-driver/blob/master/examples/mongocxx/instance_management.cpp

//#include <mongocxx/client.hpp>
#include <cstdint>
#include <iostream>
#include <vector>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/pool.hpp>

#include "vehicle.h"


class Database {
	typedef mongocxx::pool::entry connection;
	typedef mongocxx::instance instance;
	typedef mongocxx::database database;
	typedef mongocxx::collection collection;

private:
	mongocxx::uri uri;
	mongocxx::pool* pool;

public:
	Database(std::string);
	~Database();
	connection getConnection();
	void updateVehicle();





};

#endif
