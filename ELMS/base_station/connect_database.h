/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

#ifndef CONNECT_DATABASE_HPP
#define CONNECT_DATABASE_HPP

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/types/bson_value/view.hpp>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/exception/query_exception.hpp>


#include <thread>
#include <iostream>
#include <cstdint>
#include <vector>
#include <time.h>
#include <string>
#include "vehicle.h"
#include "utilities.h"


/* reference for using statement:
 *  https://jira.mongodb.org/browse/CXX-860
*/
using bsoncxx::builder::stream::finalize;
using bsoncxx::builder::stream::open_array;
using bsoncxx::builder::stream::close_array;
using std::chrono::milliseconds;
using std::string;

class Database {

    typedef mongocxx::pool::entry connection;
    typedef mongocxx::instance instance;
    typedef mongocxx::database database;
    typedef mongocxx::collection collection;
    typedef bsoncxx::builder::stream::document document;
    typedef struct bsoncxx::builder::stream::open_document_type open_document;
    typedef struct bsoncxx::builder::stream::close_document_type close_document;
private:
    mongocxx::uri uri;
    mongocxx::pool* pool;
    mongocxx::client client;
    std::thread db;

public:
    Database(std::string);
    ~Database();
    connection getConnection();

    //These two functions add an existing vehicle either with out without a pointer to the Vehicle. 
    void addVehicle(Vehicle* v);
    void updateVehicle(Vehicle*);

    //Template functions
    //Function for updating single vehicle trait. Can take in any type of variable
    //Must include unit number and variable name in mongodb
    template<typename T>
    void updateSingleVehicleTrait(std::string queryType, int unit, T value) {
        try {
            //establish pool connection
            auto connection = getConnection();
            database test = connection->database("test");
            //create vehicles
            collection vehicles = test["vehicles"];
            //store vehicles

            //Push new velocity or bearing onto appropriate array
            if (queryType == "new_velocity" || queryType == "new_bearing") {
                pushNewData(queryType, unit, value);
            }
            //if updating "new" variables, get current data to store as "last" data
            if (queryType.rfind("new", 0) == 0) {
                getPastData(queryType, unit, value);
            }
            //store new data
            bsoncxx::stdx::optional<mongocxx::result::update> result
                = vehicles.update_one(document{} << "vehicle_unit" << unit << finalize,
                    document{} << "$set" << open_document{} <<
                    queryType << value << close_document{} << finalize);
            if (result)
            {
                std::cout << "Successfully updated\n";
            }
            else
            {
                std::cout << "There was a problem updating\n";
            }
        }
        catch (mongocxx::exception& e)
        {
            std::cout << "There was an exception: " << e.what();
        }

    }

    //Pushes data onto an array. Can take in any type of variable
    //Must include unit number and variable name in mongodb
    template<typename T>
    void pushNewData(std::string queryType, int unit, T value) {
        try {
            //establish pool connection
            auto connection = getConnection();
            database test = connection->database("test");
            //create vehicles
            collection vehicles = test["vehicles"];
            //store vehicles

            std::string updateType = "";
            if (queryType == "new_velocity") {
                updateType = "past_velocity";
            }
            else {
                updateType = "past_bearing";
            }
            //push new velocity or bearing onto the array
            bsoncxx::stdx::optional<mongocxx::result::update> update_array =
                vehicles.update_one(document{}
                    << "vehicle_unit" << unit
                    << finalize,
                    document{}
                    << "$push" << open_document{}
                    << updateType << value
                    //<< "past_bearing" << new_bearing
                    << close_document{}
                    << finalize
                );
            if (update_array) {
                std::cout << "Successfully pushed\n";
            }
            else {
                std::cout << "There was a problem pushing\n";
            }
        }
        catch (mongocxx::exception& e)
        {
            std::cout << "There was an exception: " << e.what();
        }
    }

    //Queries database vehicle collection
    template <typename T>
    void queryDatabase(std::string queryType, T value) {
        try {
            //establish pool connection
            auto connection = getConnection();
            database test = connection->database("test");
            //create vehicles
            collection vehicles = test["vehicles"];
            //store vehicles
            bsoncxx::stdx::optional<bsoncxx::document::value> result
                = vehicles.find_one(document{} << queryType << value << finalize);

            if (result) {
                std::cout << bsoncxx::to_json(*result) << "\n";
            }
            else {
                std::cout << "Cannot find document";
            }
        }
        catch (mongocxx::exception& e)
        {
            std::cout << "There was an exception: " << e.what();
        }
    }
};


#endif //CONNECT_DATABASE_HPP