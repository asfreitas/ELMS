/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/


#include <bsoncxx/stdx/optional.hpp>
#include <bsoncxx/types.hpp>
#include <bsoncxx/builder/stream/document.hpp>

#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>
#include <mongocxx/stdx.hpp>

#include <iostream>
#include <cstdint>
#include <vector>
#include <time.h>

class Database {
    typedef mongocxx::pool::entry connection;
    typedef mongocxx::instance instance;
    typedef mongocxx::database database;
    typedef mongocxx::collection collection;

private:
    mongocxx::uri uri;
    mongocxx::pool* pool;
    mongocxx::client client;

public:
    Database(std::string);
    ~Database();
    connection getConnection();
    void getVehicles();
    void updateVehicle();
    void addVehicle(std::string, int, std::chrono::milliseconds, double, double, double, double, std::string);
};


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

void Database::getVehicles()
{
    //establish pool connection
    auto connection = getConnection();

    database test = connection->database("test");
    //create vehicles
    collection vehicles = test["vehicles"];
    //store vehicles
    auto findVehicles = vehicles.find({});

    for (auto doc : findVehicles)
    {
        std::cout << bsoncxx::to_json(doc) << "\n";
    }
}


//Update a vehicle: find current vehicle with unit number, 
//update last and past data with current data, then update current data
//with new data
void Database::updateVehicle()
{
    //establish pool connection
    auto connection = getConnection();

    database test = connection->database("test");
    //create vehicles
    collection vehicles = test["vehicles"];

    bsoncxx::stdx::optional<mongocxx::result::update> result = 
        vehicles.update_one(bsoncxx::builder::stream::document{} << "vehicle_unit" << 6767 << bsoncxx::builder::stream::finalize,
            bsoncxx::builder::stream::document{} << "$set" << bsoncxx::builder::stream::open_document <<
            "vehicle_unit" << 7676 << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);
}

//create a document that can be inserted using builder method
void Database::addVehicle(std::string collection_name, int unit, std::chrono::milliseconds startup_time, double new_longitude, double new_latitude, double new_velocity,
    double new_bearing, std::string status)
{

    //establish pool connection
    auto connection = getConnection();
    //create a database connection
    auto test = connection->database("test");
    auto vehicles = test["vehicles"];
    
    auto doc = bsoncxx::builder::stream::document{};
    
    bsoncxx::document::value doc_value = doc
        << "vehicle_unit" << unit
        << "startup_time" << bsoncxx::types::b_date{startup_time}
        << "last_received_time" << NULL
        << "last_longitude" << NULL
        << "last_latitude" << NULL
        << "past_velocity" << bsoncxx::builder::stream::open_array
            << NULL
        << bsoncxx::builder::stream::close_array
        << "past_bearing" << bsoncxx::builder::stream::open_array
            << NULL
        << bsoncxx::builder::stream::close_array
        << "new_time" << bsoncxx::types::b_date{ startup_time }
        << "new_longitude" << new_longitude
        << "new_latitude" << new_latitude
        << "new_velocity" << new_velocity
        << "new_bearing" << new_bearing
        << "status" << status
        << "distance_to_vehicles" << bsoncxx::builder::stream::open_array
        << bsoncxx::builder::stream::open_document
            << "vehicle_unit" << 1
            << "distance" << 75
        << bsoncxx::builder::stream::close_document
        << bsoncxx::builder::stream::close_array
        << bsoncxx::builder::stream::finalize;
        
        //obtain view of document to insert it
        bsoncxx::document::view view = doc_value.view();
        //insert the view of the document
        bsoncxx::stdx::optional<mongocxx::result::insert_one> result = vehicles.insert_one(view);
}

int main(int, char**) {

    Database newDB("mongodb+srv://asfreitas:b8_i7miJdVLAHFN@elms-cluster-k27n4.gcp.mongodb.net/test?retryWrites=true&w=majority");

    int unitNum = 6767;
    char t[26];
    std::chrono::milliseconds startup_time = std::chrono::milliseconds(1165161);
    double new_longitude = 1262.6767;
    double new_latitude = 6767.6767;
    double new_velocity = 6.7;
    double new_bearing = 6.7;
    std::string status = "active";
    std::string collection_name = "vehicles";

    //newDB.addVehicle(collection_name, unitNum, startup_time, new_longitude, new_latitude, new_velocity, new_bearing, status);
    newDB.updateVehicle();
}