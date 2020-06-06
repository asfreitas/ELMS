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
    vector<int> getAllVehicleID();

    //Template functions

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
            if (!result)
            {
                std::cout << "There was a problem updating the database.\n";
            }
        }
        catch (mongocxx::exception& e)
        {
            std::cout << "There was an internal problem with the database. Error Message:" << e.what() << std::endl;
        }

    }


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
            if (!update_array) {
                std::cout << "There was a problem updating an array in the database.\n";
            }

        }
        catch (mongocxx::exception& e)
        {
            std::cout << "There was an internal problem with the database. Error Message:" << e.what() << std::endl;
        }
    }

    template<typename T>
    void getPastData(std::string queryType, int unit, T value) {
        try {
            //establish pool connection
            auto connection = getConnection();
            database test = connection->database("test");
            //create vehicles
            collection vehicles = test["vehicles"];
            //before updating new latitude, longitude, velocity, bearing, times, grab current
            //variables and store into last latitude, longitude, velocity, bearing, times.
            bsoncxx::stdx::optional<bsoncxx::document::value> find_result =
                vehicles.find_one({ document{}
                    << "vehicle_unit" << unit
                    << finalize
                    });
            //get the document values, set value to view mode, and get element of
            //the document view mode.
            bsoncxx::document::value doc_value = *find_result;
            bsoncxx::document::view doc_view = doc_value.view();
            bsoncxx::document::element ele{ doc_view[queryType] };
            if (ele.type() == bsoncxx::type::k_utf8) {
                auto eleView = ele.get_utf8().value;
                std::string eleValue = eleView.to_string();
                updatePastData(queryType, unit, eleValue);
            }
            else if (ele.type() == bsoncxx::type::k_int32) {
                auto eleView = ele.get_int32();
                updatePastData(queryType, unit, eleView);
            }
            else if (ele.type() == bsoncxx::type::k_double) {
                auto eleView = ele.get_double();
                updatePastData(queryType, unit, eleView);
            }
            else if (ele.type() == bsoncxx::type::k_date) {
                auto eleView = ele.get_date();
                updatePastData(queryType, unit, eleView);
            }
        }
        catch (mongocxx::exception& e)
        {
            std::cout << "There was an internal problem with the database. Error Message:" << e.what() << std::endl;
        }
    }


    template<typename T>
    void updatePastData(std::string queryType, int unit, T eleView) {
        try {
            //establish pool connection
            auto connection = getConnection();
            database test = connection->database("test");
            //create vehicles
            collection vehicles = test["vehicles"];
            //replace "new" with "last" to update
            size_t index = 0;
            index = queryType.find("new", index);
            queryType.replace(index, 3, "last");
            //update past data
            bsoncxx::stdx::optional<mongocxx::result::update> result
                = vehicles.update_one(document{} << "vehicle_unit" << unit << finalize,
                    document{} << "$set" << open_document{} <<
                    queryType << eleView << close_document{} << finalize);
            if (result) {
                std::cout << "There was a problem updating the database.\n";
            }
        }
        catch (mongocxx::exception& e)
        {
            std::cout << "There was an internal problem with the database. Error Message:" << e.what() << std::endl;
        }
    }

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

            if (!result) {
                std::cout << "There was a problem querying the database\n";
            }

        }
        catch (mongocxx::exception& e)
        {
            std::cout << "There was an internal problem with the database. Error Message:" << e.what() << std::endl;
        }
    }
};


#endif //CONNECT_DATABASE_HPP