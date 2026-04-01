#include "student_service.h"
#include "../models/student.h"
#include "../utils/helper_functions.h"
#include "../config/db_config.h"
#include <iostream>

using namespace httplib;
using namespace std;
using namespace utils;

namespace services {
    void registerStudentRoutes(Server& svr) {
        svr.Get("/api/student/all", [](const Request& req, Response& res) {
            try {
                sql::Connection* conn = config::getConnection();
                sql::Statement* stmt = conn->createStatement();
                sql::ResultSet* resSet = stmt->executeQuery("SELECT * FROM students");

                string json = "[";
                bool first = true;
                while (resSet->next()) {
                    if (!first) json += ",";
                    json += "{";
                    json += "\"id\":" + to_string(resSet->getInt("id")) + ",";
                    json += "\"name\":\"" + resSet->getString("name") + "\",";
                    json += "\"age\":" + to_string(resSet->getInt("age")) + ",";
                    json += "\"course\":\"" + resSet->getString("course") + "\",";
                    json += "\"marks\":" + to_string(resSet->getDouble("marks"));
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

        svr.Post("/api/student/add", [](const Request& req, Response& res) {
            try {
                Student s;
                // Note: id might be auto-increment in MySQL
                s.name = get_json_string(req.body, "name");
                s.age = stoi(get_json_number(req.body, "age"));
                s.course = get_json_string(req.body, "course");
                s.marks = stof(get_json_number(req.body, "marks"));

                sql::Connection* conn = config::getConnection();
                sql::PreparedStatement* pstmt = conn->prepareStatement("INSERT INTO students (name, age, course, marks) VALUES (?, ?, ?, ?)");
                pstmt->setString(1, s.name);
                pstmt->setInt(2, s.age);
                pstmt->setString(3, s.course);
                pstmt->setDouble(4, s.marks);
                pstmt->execute();
                delete pstmt;
                
                cout << "[Student Added to DB] " << s.name << endl;
                res.set_content("{\"status\":\"success\"}", "application/json");
            } catch(sql::SQLException &e) {
                res.set_content("{\"status\":\"error\", \"message\":\"" + string(e.what()) + "\"}", "application/json");
            }
        });

        svr.Delete("/api/student/delete", [](const Request& req, Response& res) {
            if (req.has_param("id")) {
                try {
                    int id = stoi(req.get_param_value("id"));
                    sql::Connection* conn = config::getConnection();
                    sql::PreparedStatement* pstmt = conn->prepareStatement("DELETE FROM students WHERE id = ?");
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
