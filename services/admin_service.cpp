#include "admin_service.h"
#include "../models/admin.h"
#include "../utils/helper_functions.h"
#include "../config/db_config.h"
#include <iostream>

using namespace httplib;
using namespace std;
using namespace utils;

namespace services {
    void registerAdminRoutes(Server& svr) {
        svr.Get("/api/admin/all", [](const Request& req, Response& res) {
            try {
                sql::Connection* conn = config::getConnection();
                sql::Statement* stmt = conn->createStatement();
                sql::ResultSet* resSet = stmt->executeQuery("SELECT * FROM admin");

                string json = "[";
                bool first = true;
                while (resSet->next()) {
                    if (!first) json += ",";
                    json += "{";
                    json += "\"id\":" + to_string(resSet->getInt("id")) + ",";
                    json += "\"name\":\"" + resSet->getString("name") + "\","; 
                    json += "\"role\":\"" + resSet->getString("role") + "\""; // Quick hack mapping 
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

        svr.Post("/api/admin/add", [](const Request& req, Response& res) {
            try {
                Admin a;
                a.id = stoi(get_json_number(req.body, "id")); // Note: This might not be needed if auto_increment
                a.name = get_json_string(req.body, "name");
                a.role = get_json_string(req.body, "role");

                sql::Connection* conn = config::getConnection();
                sql::PreparedStatement* pstmt = conn->prepareStatement("INSERT INTO admin (name, role) VALUES (?, ?)");
                // Assuming "name" maps to username and "role" maps to password based on standard login mockups
                pstmt->setString(1, a.name);
                pstmt->setString(2, a.role);
                pstmt->execute();
                delete pstmt;
                
                cout << "[Admin Added to DB] " << a.name << endl;
                res.set_content("{\"status\":\"success\"}", "application/json");
            } catch(sql::SQLException &e) {
                res.set_content("{\"status\":\"error\", \"message\":\"" + string(e.what()) + "\"}", "application/json");
            }
        });

        svr.Delete("/api/admin/delete", [](const Request& req, Response& res) {
            if (req.has_param("id")) {
                try {
                    int id = stoi(req.get_param_value("id"));
                    sql::Connection* conn = config::getConnection();
                    sql::PreparedStatement* pstmt = conn->prepareStatement("DELETE FROM admin WHERE id = ?");
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
