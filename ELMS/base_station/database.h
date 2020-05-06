#pragma once

#include <bsoncxx/builder/stream/document.hpp>
#include <bsoncxx/json.hpp>
#include <mongocxx/client.hpp>
#include <mongocxx/instance.hpp>
#include <mongocxx/logger.hpp>
#include <mongocxx/pool.hpp>
#include <mongocxx/uri.hpp>

//ref https://github.com/mongodb/mongo-cxx-driver/blob/master/examples/mongocxx/instance_management.cpp

namespace {

    // The mongocxx::instance constructor and destructor initialize and shut down the driver,
    // respectively. Therefore, a mongocxx::instance must be created before using the driver and
    // must remain alive for as long as the driver is in use.
    //
    // This example demonstrates how one might keep a heap allocated mongocxx::instance and
    // mongocxx::pool object associated in a way that allows dynamic configuration. Most of the examples
    // simply create a stack allocated mongocxx::instance object, which is fine for small examples, but
    // real programs will typically require shared access to a commonly configured instance and
    // connection pool across different translation units and components. By providing a singleton which
    // owns the instance and pool objects, we can defer configuration until we are ready to use MongoDB,
    // and share the instance and pool objects between multiple functions.

    class mongo_access {
    public:
        static mongo_access& instance() {
            static mongo_access instance;
            return instance;
        }

        void configure(std::unique_ptr<mongocxx::instance> instance,
            std::unique_ptr<mongocxx::pool> pool) {
            _instance = std::move(instance);
            _pool = std::move(pool);
        }

        using connection = mongocxx::pool::entry;

        connection get_connection() {
            return _pool->acquire();
        }

        bsoncxx::stdx::optional<connection> try_get_connection() {
            return _pool->try_acquire();
        }

    private:
        mongo_access() = default;

        std::unique_ptr<mongocxx::instance> _instance = nullptr;
        std::unique_ptr<mongocxx::pool> _pool = nullptr;
    };

}  // namespace