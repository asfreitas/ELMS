/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/

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
    typedef bsoncxx::builder::stream::close_document_type close_document;
    
private:
    mongocxx::uri uri;
    mongocxx::pool* pool;
    mongocxx::client client;

public:
    Database(std::string);
    ~Database();
    connection getConnection();
    void getVehicles();
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
