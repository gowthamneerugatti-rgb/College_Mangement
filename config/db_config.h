#pragma once
#include <string>
#include <cppconn/driver.h>
#include <cppconn/connection.h>
#include <cppconn/exception.h>
#include <cppconn/resultset.h>
#include <cppconn/statement.h>
#include <cppconn/prepared_statement.h>

namespace config {
    struct DbConfig {
        std::string host = "tcp://db:3306";
        std::string user = "root";
        std::string password = "rootpassword";
        std::string db_name = "college_db";
    };

    void initDatabase(const DbConfig& config);
    sql::Connection* getConnection();
}
