/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/


#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/types/bson_value/view.hpp>
#include <bsoncxx/builder/stream/document.hpp>


#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>
#include <mongocxx/exception/query_exception.hpp>
#include <bsoncxx/types.hpp>

#include <thread>
#include <iostream>
#include <cstdint>
#include <vector>
#include <time.h>



class Database {

    typedef mongocxx::pool::entry connection;
    typedef mongocxx::instance instance;
    typedef mongocxx::database database;
    typedef mongocxx::collection collection;
    typedef bsoncxx::builder::stream::document document;
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
    void updateVehicle(std::string collection_name, int unit, std::chrono::milliseconds startup_time, double new_longitude, double new_latitude, double new_velocity,
        double new_bearing, std::string status);
    void addVehicle(std::string collection_name, int unit, std::chrono::milliseconds startup_time, double new_longitude, double new_latitude, double new_velocity,
        double new_bearing, std::string status);
    template <typename T>
    void queryDatabase(std::string, T);
    template <typename T>
    void updateSingleVehicleTrait(std::string, int, T);
    template <typename T>
    void pushNewData(std::string, int, T);
    template <typename T>
    void getPastData(std::string, int, T);
    template <typename T>
    void updatePastData(std::string, int, T);
};