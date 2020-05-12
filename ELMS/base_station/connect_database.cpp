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
void Database::updateVehicle(std::string collection_name, int unit, std::chrono::milliseconds message_time, double new_longitude, double new_latitude, double new_velocity,
    double new_bearing, std::string status)
{
    //establish pool connection
    auto connection = getConnection();

    database test = connection->database("test");
    //create vehicles
    collection vehicles = test[collection_name];
    
    
    bsoncxx::stdx::optional<bsoncxx::document::value> find_result =
        vehicles.find_one({ bsoncxx::builder::stream::document{}
            << "vehicle_unit" << unit
            << bsoncxx::builder::stream::finalize
        });

    if (find_result) {
        std::cout << bsoncxx::to_json(*find_result) << "\n";
    }

    bsoncxx::document::value doc_value = *find_result;

    bsoncxx::document::view doc_view = doc_value.view();

   // std::cout << bsoncxx::to_json(doc_value["status"]) << "\n";

    //print out string
    bsoncxx::document::element ele{ doc_view["status"] };
    auto eleView = ele.get_utf8().value;
    std::string eleValue = eleView.to_string();
    std::cout << eleValue << "\n";

    //print out int
    bsoncxx::document::element eleUnit{ doc_view["vehicle_unit"] };
    auto uniView = eleUnit.get_int32();
    std::cout << uniView << "\n";
    
    //print out double
    bsoncxx::document::element eleLat{ doc_view["new_latitude"] };
    auto latView = eleLat.get_double();
    std::cout << latView << "\n";


    //print out date time
    bsoncxx::document::element eleStart{ doc_view["startup_time"] };
    auto startView = eleStart.get_date();
    std::cout << startView << "\n";


    /*
    //push element onto past_velocity array
    bsoncxx::stdx::optional<mongocxx::result::update> update_velocity =
        vehicles.update_one(bsoncxx::builder::stream::document{}
            << "vehicle_unit" << unit
            << bsoncxx::builder::stream::finalize, 
            bsoncxx::builder::stream::document{}
            << "$push" << bsoncxx::builder::stream::open_document
            << "past_velocity" << new_velocity
            //<< "past_bearing" << new_bearing
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::finalize
        );

    //push element onto past_bearing array
    bsoncxx::stdx::optional<mongocxx::result::update> update_bearing =
        vehicles.update_one(bsoncxx::builder::stream::document{}
            << "vehicle_unit" << unit
            << bsoncxx::builder::stream::finalize,
            bsoncxx::builder::stream::document{}
            << "$push" << bsoncxx::builder::stream::open_document
            << "past_bearing" << new_bearing
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::finalize
        );

    //Update all information besides arrays
    bsoncxx::stdx::optional<mongocxx::result::update> result = 
        vehicles.update_one(bsoncxx::builder::stream::document{} 
            << "vehicle_unit" << unit
            << bsoncxx::builder::stream::finalize,
            bsoncxx::builder::stream::document{} 
            << "$set" << bsoncxx::builder::stream::open_document 
            //don't really need, just for testing purposes.    
//          << "vehicle_unit" << 9001

            //figure out how to update these.
            << "startup_time" << bsoncxx::types::b_date{ message_time }
            << "last_received_time" << bsoncxx::types::b_date{ message_time }
            << "last_longitude" << NULL
            << "last_latitude" << NULL

            << "new_time" << bsoncxx::types::b_date{ message_time }
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
            << bsoncxx::builder::stream::open_document
            << "vehicle_unit" << 2
            << "distance" << 90
            << bsoncxx::builder::stream::close_document
            << bsoncxx::builder::stream::close_array
            << bsoncxx::builder::stream::close_document 
            << bsoncxx::builder::stream::finalize
        );
        if (result && update_velocity && update_bearing) {
            std::cout << "Vehicle succuessfully updated" << "\n";
        }
        */
}



void Database::updateVehicleStartupTime(std::string collection_name, int unit, std::chrono::milliseconds startup_time) {

}


void updateVehicleLastReceiveTime(std::string collection_name, int unit, std::chrono::milliseconds last_receive_time) {

}
void updateVehicleLastLongitude(std::string collection_name, int unit) {

}
void updateVehicleLastLatitude(std::string collection_name, int unit) {

}

void updateVehicleNewTime(std::string collection_name, int unit, std::chrono::milliseconds new_time) {

}

void updateVehicleNewLongitude(std::string collection_name, int unit, double new_longitude) {

}

void updateVehicleNewLatitude(std::string collection_name, int unit, double new_latitude) {

}

void updateVehicleNewVelocity(std::string collection_name, int unit, double new_velocity) {

}

void updateVehicleNewBearing(std::string collection_name, int unit, double new_bearing) {

}

void updateVehicleStatus(std::string collection_name, int unit, std::string status) {

}

void updateVehicleDistanceToVehicles(std::string collection_name, int unit) {

}







//create a document that can be inserted using builder method
void Database::addVehicle(std::string collection_name, int unit, std::chrono::milliseconds message_time, double new_longitude, double new_latitude, double new_velocity,
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
        << "startup_time" << bsoncxx::types::b_date{ message_time }
        << "last_received_time" << NULL
        << "last_longitude" << NULL
        << "last_latitude" << NULL
        << "past_velocity" << bsoncxx::builder::stream::open_array
            << new_velocity
        << bsoncxx::builder::stream::close_array
        << "past_bearing" << bsoncxx::builder::stream::open_array
            << new_bearing
        << bsoncxx::builder::stream::close_array
        << "new_time" << bsoncxx::types::b_date{ message_time }
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

        if (result) {
            std::cout << "Vehicle succuessfully created" << "\n";
        }

}

int main(int, char**) {

    Database newDB("mongodb+srv://asfreitas:b8_i7miJdVLAHFN@elms-cluster-k27n4.gcp.mongodb.net/test?retryWrites=true&w=majority");

    int unitNum = 1001;
    char t[26];
    std::chrono::milliseconds message_time = std::chrono::milliseconds(5236521);
    double new_longitude = 9876.5432;
    double new_latitude = 1234.5678;
    double new_velocity = 67;
    double new_bearing = 7.6;
    std::string status = "inactive";
    std::string collection_name = "vehicles";

    //newDB.addVehicle(collection_name, unitNum, startup_time, new_longitude, new_latitude, new_velocity, new_bearing, status);
    newDB.updateVehicle(collection_name, unitNum, message_time, new_longitude, new_latitude, new_velocity, new_bearing, status);
}