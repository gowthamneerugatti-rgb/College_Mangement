#include "db_config.h"
#include <iostream>
#include <cstdlib>
#include <chrono>
#include <thread>

namespace config {

    // Global singleton connection
    static sql::Connection* global_conn = nullptr;

    void initDatabase(const DbConfig& config) {
        std::cout << "[Database] Initializing MySQL database connection to " << config.host << "..." << std::endl;
        
        sql::Driver* driver = get_driver_instance();
        int retries = 10;
        
        while (retries > 0) {
            try {
                global_conn = driver->connect(config.host, config.user, config.password);
                global_conn->setSchema(config.db_name);
                std::cout << "[Database] Connected to " << config.db_name << " successfully." << std::endl;
                return;
            } catch (sql::SQLException &e) {
                std::cerr << "[Database] Connection failed: " << e.what() << " - Retrying in 3s..." << std::endl;
                std::this_thread::sleep_for(std::chrono::seconds(3));
                retries--;
            }
        }
        
        std::cerr << "[Database] FATAL ERROR: Could not connect to database after 10 retries." << std::endl;
        exit(1);
    }

    sql::Connection* getConnection() {
        return global_conn;
    }

}
