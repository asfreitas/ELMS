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
    typedef bsoncxx::builder::stream::close_document close_document;


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
    void updateVehicleStartupTime(std::string collection_name, int unit, std::chrono::milliseconds startup_time);
    void updateVehicleLastReceiveTime(std::string collection_name, int unit, std::chrono::milliseconds last_receive_time);
    void updateVehicleLastLongitude(std::string collection_name, int unit);
    void updateVehicleLastLatitude(std::string collection_name, int unit);
    void updateVehiclePastVelocity(std::string collection_name, int unit);
    void updateVehiclePastBearing(std::string collection_name, int unit);
    void updateVehicleNewTime(std::string collection_name, int unit);
    void updateVehicleNewLongitude(std::string collection_name, int unit);
    void updateVehicleNewLatitude(std::string collection_name, int unit);
    void updateVehicleNewVelocity(std::string collection_name, int unit);
    void updateVehicleNewBearing(std::string collection_name, int unit);
    void updateVehicleStatus(std::string collection_name, int unit);
    void updateVehicleDistanceToVehicles(std::string collection_name, int unit);
    void addVehicle(std::string collection_name, int unit, std::chrono::milliseconds startup_time, double new_longitude, double new_latitude, double new_velocity,
        double new_bearing, std::string status);
    template <typename T>
    void queryDatabase(std::string, T);
    template <typename T>
    void updateSingleVehicleTrait(std::string, int, T);
};
