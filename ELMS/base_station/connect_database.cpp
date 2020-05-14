/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
*/




#include "connect_database.h"

//constructor for pool instance used for threading connections
Database::Database(std::string _uri)
{
    static instance instance{};
    uri = mongocxx::uri(_uri);
    pool = new mongocxx::pool{ uri };
}

//desctructor
Database::~Database()
{
    std::cout << "This can't go out of scope until the"
        "program closes otherwise we lose the connection";
    delete pool;
}

//create a connection pool
Database::connection Database::getConnection()
{
    return pool->acquire();
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

}

template<typename T>
void Database::updateSingleVehicleTrait(std::string queryType, int unit, T value){
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
        = vehicles.update_one(document{} << "vehicle_unit" << unit << bsoncxx::builder::stream::finalize,
            document{} << "$set" << bsoncxx::builder::stream::open_document <<
            queryType << value << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);
    if (result)
    {
        std::cout << "Successfully updated\n";
    }
    else
    {
        std::cout << "There was a problem updating\n";
    }

}


template<typename T>
void Database::pushNewData(std::string queryType, int unit, T value) {
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
        vehicles.update_one(bsoncxx::builder::stream::document{}
            << "vehicle_unit" << unit
            << bsoncxx::builder::stream::finalize,
            bsoncxx::builder::stream::document{}
            << "$push" << bsoncxx::builder::stream::open_document
            << updateType << value
            //<< "past_bearing" << new_bearing
            << close_document{}
            << bsoncxx::builder::stream::finalize
        );
    if (update_array){
        std::cout << "Successfully pushed\n";
    }
    else{
        std::cout << "There was a problem pushing\n";
    }
}

template<typename T>
void Database::getPastData(std::string queryType, int unit, T value) {
    //establish pool connection
    auto connection = getConnection();
    database test = connection->database("test");
    //create vehicles
    collection vehicles = test["vehicles"];
    //before updating new latitude, longitude, velocity, bearing, times, grab current
    //variables and store into last latitude, longitude, velocity, bearing, times.
    bsoncxx::stdx::optional<bsoncxx::document::value> find_result =
        vehicles.find_one({ bsoncxx::builder::stream::document{}
            << "vehicle_unit" << unit
            << bsoncxx::builder::stream::finalize
            });
    //get the document values, set value to view mode, and get element of
    //the document view mode.
    bsoncxx::document::value doc_value = *find_result;
    bsoncxx::document::view doc_view = doc_value.view();
    bsoncxx::document::element ele{ doc_view[queryType] };
    if(ele.type() == bsoncxx::type::k_utf8) {
        auto eleView = ele.get_utf8().value;
        std::string eleValue = eleView.to_string();
        updatePastData(queryType, unit, eleValue);
    }else if(ele.type() == bsoncxx::type::k_int32){
        auto eleView = ele.get_int32();
        updatePastData(queryType, unit, eleView);
    }else if(ele.type() == bsoncxx::type::k_double){
        auto eleView = ele.get_double();
        updatePastData(queryType, unit, eleView);
    }else if(ele.type() == bsoncxx::type::k_date) {
        auto eleView = ele.get_date();
        updatePastData(queryType, unit, eleView);
    }   
}


template<typename T>
void Database::updatePastData(std::string queryType, int unit, T eleView) {
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
        = vehicles.update_one(document{} << "vehicle_unit" << unit << bsoncxx::builder::stream::finalize,
            document{} << "$set" << bsoncxx::builder::stream::open_document <<
            queryType << eleView << bsoncxx::builder::stream::close_document << bsoncxx::builder::stream::finalize);
    if (result) {
        std::cout << "Successfully updated\n";
    }
    else {
        std::cout << "There was a problem updating\n";
    }
}

template <typename T>
void Database::queryDatabase(std::string queryType, T value){
    try
    {
        //establish pool connection
        auto connection = getConnection();
        database test = connection->database("test");
        //create vehicles
        collection vehicles = test["vehicles"];
        //store vehicles
        bsoncxx::stdx::optional<bsoncxx::document::value> result
            = vehicles.find_one(bsoncxx::builder::stream::document{} << queryType << value << bsoncxx::builder::stream::finalize);

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


//create a document that can be inserted using builder method
void Database::addVehicle(std::string collection_name, int unit, std::chrono::milliseconds message_time, double new_longitude, double new_latitude, double new_velocity,
    double new_bearing, std::string status){
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
    else {
        std::cout << "Unsuccessful with creating" << "\n";
    }
}

int main(int, char**) {

    Database newDB("mongodb+srv://asfreitas:b8_i7miJdVLAHFN@elms-cluster-k27n4.gcp.mongodb.net/test?retryWrites=true&w=majority");
    /*
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
    */

    newDB.updateSingleVehicleTrait("new_velocity", 1001, 17);
    newDB.queryDatabase("vehicle_unit", 1);

}