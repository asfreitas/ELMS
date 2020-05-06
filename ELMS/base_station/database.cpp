#pragma once

#include "database.h"
#include "vehicle.h"

void updateVehicle(Vehicle* vehicle)
{
	auto connection = mongo_access::instance().get_connection();
}

void addVehicle(Vehicle* vehicle)
{
	auto connection = mongo_access::instance().get_connection();

	connection->database("test").collection("vehicles").find({});
}

void configure(mongocxx::uri uri) {
    class noop_logger : public mongocxx::logger {
    public:
        virtual void operator()(mongocxx::log_level,
            bsoncxx::stdx::string_view,
            bsoncxx::stdx::string_view) noexcept {}
    };

    auto instance =
        bsoncxx::stdx::make_optional<mongocxx::instance>(bsoncxx::stdx::make_unique<noop_logger>());

    mongo_access::instance().configure(std::move(instance),
        bsoncxx::stdx::make_unique<mongocxx::pool>(std::move(uri)));
    bsoncxx::stdx::make_optional<
}