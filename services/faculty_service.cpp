#include "faculty_service.h"
#include "../models/faculty.h"
#include "../utils/helper_functions.h"
#include "../config/db_config.h"
#include <iostream>

using namespace httplib;
using namespace std;
using namespace utils;

namespace services {
    void registerFacultyRoutes(Server& svr) {
        svr.Get("/api/faculty/all", [](const Request& req, Response& res) {
            try {
                sql::Connection* conn = config::getConnection();
                sql::Statement* stmt = conn->createStatement();
                sql::ResultSet* resSet = stmt->executeQuery("SELECT * FROM faculty"); // Assuming table is Faculty

                string json = "[";
                bool first = true;
                while (resSet->next()) {
                    if (!first) json += ",";
                    json += "{";
                    json += "\"id\":" + to_string(resSet->getInt("id")) + ",";
                    json += "\"name\":\"" + resSet->getString("name") + "\",";
                    json += "\"department\":\"" + resSet->getString("department") + "\",";
                    json += "\"designation\":\"" + resSet->getString("designation") + "\",";
                    json += "\"salary\":" + to_string(resSet->getDouble("salary"));
                    json += "}";
                    first = false;
                }
                json += "]";
                delete resSet;
                delete stmt;
                res.set_content(json, "application/json");
            } catch(sql::SQLException &e) {
                res.set_content("{\"status\":\"error\", \"message\":\"" + string(e.what()) + "\"}", "application/json");
            }
        });

        svr.Post("/api/faculty/add", [](const Request& req, Response& res) {
            try {
                Faculty f;
                f.name = get_json_string(req.body, "name");
                f.department = get_json_string(req.body, "department");
                f.designation = get_json_string(req.body, "designation");
                f.salary = stof(get_json_number(req.body, "salary"));

                sql::Connection* conn = config::getConnection();
                sql::PreparedStatement* pstmt = conn->prepareStatement("INSERT INTO faculty (name, department, designation, salary) VALUES (?, ?, ?, ?)");
                pstmt->setString(1, f.name);
                pstmt->setString(2, f.department);
                pstmt->setString(3, f.designation);
                pstmt->setDouble(4, f.salary);
                pstmt->execute();
                delete pstmt;
                
                cout << "[Faculty Added to DB] " << f.name << endl;
                res.set_content("{\"status\":\"success\"}", "application/json");
            } catch(sql::SQLException &e) {
                res.set_content("{\"status\":\"error\", \"message\":\"" + string(e.what()) + "\"}", "application/json");
            }
        });

        svr.Delete("/api/faculty/delete", [](const Request& req, Response& res) {
            if (req.has_param("id")) {
                try {
                    int id = stoi(req.get_param_value("id"));
                    sql::Connection* conn = config::getConnection();
                    sql::PreparedStatement* pstmt = conn->prepareStatement("DELETE FROM faculty WHERE id = ?");
                    pstmt->setInt(1, id);
                    pstmt->execute();
                    delete pstmt;
                } catch(sql::SQLException &e) {
                    res.set_content("{\"status\":\"error\", \"message\":\"" + string(e.what()) + "\"}", "application/json");
                    return;
                }
            }
            res.set_content("{\"status\":\"deleted\"}", "application/json");
        });
    }
}
