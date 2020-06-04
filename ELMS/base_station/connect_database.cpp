/*
* ELMS - Trevor Frame, Andrew Freitas, Deborah Kretzschmar
* Contains functions for conencting to MongoDB Atlas,
* Create and add vehicle information, and update vehicle information.
*/

#include "connect_database.h"
static int count = 0;

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
    delete pool;
}

//create a connection pool
Database::connection Database::getConnection()
{
    return pool->acquire();
}

//create a document that can be inserted using builder method
void Database::addVehicle(Vehicle* vehicle) {

    int currentTime = vehicle->getTime();

    //get date-time using the zulu time in the vehicle
    time_t currentDateTime = zuluToDate(currentTime);
    milliseconds current_message_time = milliseconds(currentDateTime * 1000);

    try {
        //establish pool connection
        auto connection = getConnection();
        //create a database connection
        auto test = connection->database("test");
        auto vehicles = test["vehicles"];

        //get a map of distance to vehicles vector
        map<int, double>* mapVehicles = vehicle->getMapOfVehicles();

        bsoncxx::builder::stream::document builder{};
        //first, build distance_to_vehicles array with objects of other vehicles
        auto sub_array = builder << "distance_to_vehicles" << open_array;
        for (pair<int, double> element : *mapVehicles) {
            sub_array = sub_array << open_document{}
                << "vehicle_unit" << element.first
                << "distance" << element.second
                << close_document{};
        }
		//fill in remaining data to be stored into mongodb
        auto after_array = sub_array << close_array;
        after_array
            << "vehicle_unit" << vehicle->getUnit()
            << "startup_time" << bsoncxx::types::b_date{ current_message_time }
            << "last_received_time" << NULL
            << "last_longitude" << NULL
            << "last_latitude" << NULL
            << "past_velocity" << open_array
            << vehicle->getVelocity()
            << close_array
            << "past_bearing" << open_array
            << vehicle->getBearing()
            << close_array
            << "new_time" << bsoncxx::types::b_date{ current_message_time }
            << "new_longitude" << vehicle->getLongitude()
            << "new_latitude" << vehicle->getLatitude()
            << "new_velocity" << vehicle->getVelocity()
            << "new_bearing" << vehicle->getBearing()
            << "status" << vehicle->getStatus()
            << "priority" << vehicle->getPriorityNumber();
            bsoncxx::document::value doc_view = after_array << finalize;

        //obtain view of document to insert it
        bsoncxx::document::view view = doc_view.view();
        //insert the view of the document
        bsoncxx::stdx::optional<mongocxx::result::insert_one> result = vehicles.insert_one(view);
        if (!result) {
            std::cout << "Cannot create vehicle in database. It might already exist or there is a problem connecting." << "\n";
        }

        
    }
    catch (mongocxx::exception& e)
    {
        std::cout << "There was an internal problem with the database. Error Message:" << e.what() << std::endl;
    }
}

//Update a vehicle: find current vehicle with unit number, 
//update last and past data with current data, then update current data
//with new data
void Database::updateVehicle(Vehicle* vehicle)
{
    try
    {
        //establish pool connection
        auto connection = getConnection();
        //create a database connection
        auto test = connection->database("test");
        auto vehicles = test["vehicles"];


        int currentTime = vehicle->getTime();
        int lastTime = vehicle->getPreviousTime();

        //get date-time using the zulu time in the vehicle
        time_t currentDateTime = zuluToDate(currentTime);
        milliseconds current_message_time = milliseconds(currentDateTime * 1000);

        time_t lastDateTime = zuluToDate(lastTime);
        milliseconds last_message_time = milliseconds(lastDateTime * 1000);

		//If vehicle status now offline, set startup_time to null
        if (vehicle->getStatus() == "offline") {
            bsoncxx::stdx::optional<mongocxx::result::update> update_startup =
                vehicles.update_one(document{}
                    << "vehicle_unit" << vehicle->getUnit()
                    << finalize,
                    document{}
                    << "$set" << open_document{}
                    << "startup_time" << NULL
                    << close_document{}
                    << finalize
                );
            vehicle->setStartupTime(NULL);
        }
        //else if status is not "offline" and the startup time is 0, reset startup time
        else if(vehicle->getStartupTime() == NULL){
            bsoncxx::stdx::optional<mongocxx::result::update> update_startup =
                vehicles.update_one(document{}
                    << "vehicle_unit" << vehicle->getUnit()
                    << finalize,
                    document{}
                    << "$set" << open_document{}
                    << "startup_time" << bsoncxx::types::b_date{ current_message_time }
                    << close_document{}
                    << finalize
                );
            vehicle->setStartupTime(vehicle->getTime());
        }

        //get a map of distance to vehicles vector
        map<int, double>* mapVehicles = vehicle->getMapOfVehicles();

        bsoncxx::builder::stream::document builder{};
        //first, build distance_to_vehicles array with objects of other vehicles
        auto sub_array = builder << "$set" << open_document{} 
        << "distance_to_vehicles" << open_array;
        for (pair<int, double> element : *mapVehicles) {
            sub_array = sub_array << open_document{}
                << "vehicle_unit" << element.first
                << "distance" << element.second
                << close_document{};
        }
		//fill in remaining data to be set into mongodb
        auto after_array = sub_array << close_array;
        after_array
            << "vehicle_unit" << vehicle->getUnit()
            << "last_received_time" << bsoncxx::types::b_date{ last_message_time }
            << "last_longitude" << vehicle->getPreviousLongitude()
            << "last_latitude" << vehicle->getPreviousLatitude()
            << "new_time" << bsoncxx::types::b_date{ current_message_time }
            << "new_longitude" << vehicle->getLongitude()
            << "new_latitude" << vehicle->getLatitude()
            << "new_velocity" << vehicle->getVelocity()
            << "new_bearing" << vehicle->getBearing()
            << "status" << vehicle->getStatus()
            << "priority" << vehicle->getPriorityNumber();
        bsoncxx::document::value doc_view = after_array << close_document{} << finalize;
        bsoncxx::document::view view = doc_view.view();
		//update the vehicle
        auto result = vehicles.update_one(
            document{} << "vehicle_unit" << vehicle->getUnit() << finalize,
            view);
        if (!result)
        {
            std::cout << "There was a problem updating the vehicle in the database.\n";
        }
		//push data onto the arrays
        bsoncxx::stdx::optional<mongocxx::result::update> update_array =
            vehicles.update_one(document{}
                << "vehicle_unit" << vehicle->getUnit()
                << finalize,
                document{}
                << "$push" << open_document{}
                    << "past_velocity" << vehicle->getVelocity()
                    << "past_bearing" << vehicle->getBearing()
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


vector<int> Database::getAllVehicleID()
{
    vector<int> vehicle_id;

    try
    {
        //establish pool connection
        auto connection = getConnection();
        //create a database connection
        auto test = connection->database("test");
        auto vehicles = test["vehicles"];

        mongocxx::pipeline p{};
        p.project(bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("vehicle_unit", 1))); // only get the vehicle_unit
        p.project(bsoncxx::builder::basic::make_document(
            bsoncxx::builder::basic::kvp("_id", 0))); // but we don't want any id numbers

        auto cursor =
            vehicles.aggregate(p, mongocxx::options::aggregate{});

        for (auto doc : cursor)
        {
            string string;
            string = bsoncxx::to_json(doc);
            string[string.length()-1] = 0; // add null terminator
            int pos = string.find(':'); // e only want the number here so we take it off at the :
            string = string.substr(pos + 1, string.length()-1); // find number

            vehicle_id.push_back(stringToInt(string));
        }
    }
    catch(mongocxx::exception& e)
    {
        std::cout << "There was an internal problem with the database. Error Message:" << e.what() << std::endl;

    }
    return vehicle_id;
}
