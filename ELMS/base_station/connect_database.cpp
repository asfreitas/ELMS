/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
This file provides the Database class definitions
*/
#include "connect_database.h"
static int count = 0;
/*
============
Database

constructor for pool instance used for threading connections
============
*/
Database::Database(std::string _uri)
{
    static instance instance{};
    uri = mongocxx::uri(_uri);
    pool = new mongocxx::pool{ uri };
}



/*
===============
~Database

destructor
===============
*/
Database::~Database()
{
    std::cout << "This can't go out of scope until the"
        "program closes otherwise we lose the connection";
    delete pool;
}



/*
===============
getConnection

create a connection pool
===============
*/
Database::connection Database::getConnection()
{
    count++;
    std::cout << "The count is: " << count << endl;
    return pool->acquire();
}


/*
===============
addVehicle

Arguments: a pointer to a vehicle

create a document that can be inserted using builder method
===============
*/
void Database::addVehicle(Vehicle* vehicle) {

    try {
        //establish pool connection
        cout << "Going to call getConnection from addVehicle" << endl;
        auto connection = getConnection();
        //create a database connection
        auto test = connection->database("test");
        auto vehicles = test["vehicles"];

        //get a map of distance to vehicles vector
        map<int, double>* mapVehicles = vehicle->getMapOfVehicles();

        bsoncxx::builder::stream::document builder{};
        //first, build distance_to_vehicles array with objects of other vehicles
        auto sub_array = builder << "distance_to_vehicles" << bsoncxx::builder::stream::open_array;
        for (pair<int, double> element : *mapVehicles) {
            sub_array = sub_array << bsoncxx::builder::stream::open_document
                << "vehicle_unit" << element.first
                << "distance" << element.second
                << bsoncxx::builder::stream::close_document;
        }
        auto after_array = sub_array << bsoncxx::builder::stream::close_array;
        after_array
            << "vehicle_unit" << vehicle->getUnit()
            //<< "startup_time" << bsoncxx::types::b_date{ message_time }
            << "last_received_time" << NULL
            << "last_longitude" << NULL
            << "last_latitude" << NULL
            << "past_velocity" << open_array
            << vehicle->getVelocity()
            << close_array
            << "past_bearing" << open_array
            << vehicle->getBearing()
            << close_array
            //<< "new_time" << bsoncxx::types::b_date{ message_time }
            << "new_longitude" << roundToFourDecimals(vehicle->getLongitude())
            << "new_latitude" << roundToFourDecimals(vehicle->getLatitude())
            << "new_velocity" << vehicle->getVelocity()
            << "new_bearing" << vehicle->getBearing()
            << "status" << vehicle->getStatus();
        bsoncxx::document::value doc_view = after_array << finalize;

        //obtain view of document to insert it
        bsoncxx::document::view view = doc_view.view();
        //insert the view of the document
        bsoncxx::stdx::optional<mongocxx::result::insert_one> result = vehicles.insert_one(view);
        if (result) {
            std::cout << "Vehicle successfully created" << "\n";
        }
        else {
            std::cout << "Unsuccessful with creating" << "\n";
        }

    }
    catch (mongocxx::exception& e)
    {
        std::cout << "There was an exception: " << e.what();
    }
}




/*
===============
updateVehicle

Arguments: A pointer to a vehcile

Update a vehicle: find current vehicle with unit number,
update last and past data with current data, then update current data
with new data
===============
*/
void Database::updateVehicle(Vehicle* vehicle)
{
    try
    {
        //establish pool connection
        cout << "Going to call getConnection from updateVehicle" << endl;
        auto connection = getConnection();
        //create a database connection
        auto test = connection->database("test");
        auto vehicles = test["vehicles"];

        //get a map of distance to vehicles vector
        map<int, double>* mapVehicles = vehicle->getMapOfVehicles();

        auto result = vehicles.update_one(
            document{} << "vehicle_unit" << vehicle->getUnit() << finalize,
            document{} << "$push" << open_document{}
            << "past_velocity" << vehicle->getVelocity()
            << "past_bearing" << vehicle->getBearing()
            << close_document{}
            << "$set" << open_document{}
            << "new_longitude" << roundToFourDecimals(vehicle->getLongitude())
            << "new_latitude" << roundToFourDecimals(vehicle->getLatitude())
            << "new_velocity" << vehicle->getVelocity()
            << "new_bearing" << vehicle->getBearing()
            //<< "new_time" << bsoncxx::types::b_date{ messageTime }
            << "status" << vehicle->getStatus()
            << close_document{} << finalize);
        if (result)
        {
            std::cout << "Successfully updated vehicle\n";
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
/*
===============
updateSingleVehicleTrait

Arguments: (1) a queryType such as "vehicle_unit", "past_velocity" ,"past_bearing",
            "new_longitude", "new_latitude", "new_velocity","new_bearing","new_time:,
            "status"
            (2) an int that represents the vehicle unit id number
            (3) the value for update

Updates one specific vehicle trait in the database
===============
*/
template<typename T>
void Database::updateSingleVehicleTrait(std::string queryType, int unit, T value) {
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


/*
===============
pushNewData

Arguments: (1) a queryType such as "vehicle_unit", "past_velocity" ,"past_bearing",
            "new_longitude", "new_latitude", "new_velocity","new_bearing","new_time:,
            "status"
            (2) an int that represents the vehicle unit id number
            (3) the value to be pushed

Updates the database with new data
===============
*/
template<typename T>
void Database::pushNewData(std::string queryType, int unit, T value) {
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

/*
===============
getPastData

Arguments: (1) a queryType such as "vehicle_unit", "past_velocity" ,"past_bearing",
            "new_longitude", "new_latitude", "new_velocity","new_bearing","new_time:,
            "status"
            (2) an int that represents the vehicle unit id number
            (3) the value to be retrieved

Allows user to retrieve previous vehicle information
===============
*/
template<typename T>
void Database::getPastData(std::string queryType, int unit, T value) {
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
        std::cout << "There was an exception: " << e.what();
    }
}


/*
===============
updatePastData

Arguments: (1) a queryType such as "vehicle_unit", "past_velocity" ,"past_bearing",
            "new_longitude", "new_latitude", "new_velocity","new_bearing","new_time:,
            "status"
            (2) an int that represents the vehicle unit id number
            (3) the value to be placed into past data

Update past data
===============
*/
template<typename T>
void Database::updatePastData(std::string queryType, int unit, T eleView) {
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
            std::cout << "Successfully updated\n";
        }
        else {
            std::cout << "There was a problem updating\n";
        }
    }
    catch (mongocxx::exception& e)
    {
        std::cout << "There was an exception: " << e.what();
    }
}

/*
===============
queryDatabase

Arguments: (1) a queryType such as "vehicle_unit", "past_velocity" ,"past_bearing",
            "new_longitude", "new_latitude", "new_velocity","new_bearing","new_time:,
            "status"
            (2) the value to be queried for

Displays requested information based for queryType. 
For example, if querytype = "vehicle_unit" and T value = 350, information
about vehicle unit 350 will be displayed. 
===============
*/
template <typename T>
void Database::queryDatabase(std::string queryType, T value) {
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
/*
int main()
{
    Database newDB("mongodb+srv://asfreitas:b8_i7miJdVLAHFN@elms-cluster-k27n4.gcp.mongodb.net/test?retryWrites=true&w=majority");




    Vehicle* v1 = new Vehicle(9997, 152632, 1234.1234, 1223.1234, 400, 800, 1, "inactive");
    v1->updateVehicleMap(v1, 350, 60);
    v1->updateVehicleMap(v1, 9997, 60);
    v1->updateVehicleMap(v1, 67, 50);


    Vehicle* v2 = new Vehicle(350, 152632, 1010.1010, 2020.2020, 9, 9, 1, "inactive");

    newDB.addVehicle(v1);
    //newDB.updateVehicle(v2);


   newDB.updateSingleVehicleTrait("new_velocity", 67, 50);

   return 0;
}*/

