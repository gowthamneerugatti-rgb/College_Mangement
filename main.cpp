#include "httplib.h"
#include <iostream>
#include "config/db_config.h"
#include "services/admin_service.h"
#include "services/student_service.h"
#include "services/faculty_service.h"

using namespace httplib;
using namespace std;

int main() {
    // 1. Initialize DB Config
    config::DbConfig dbCfg;
    config::initDatabase(dbCfg);

    // 2. Create Web Server
    Server svr;
    
    // Serve Static HTML Files from the public folder
    svr.set_mount_point("/", "./public");

    // 3. Register Application API Routes
    services::registerAdminRoutes(svr);
    services::registerStudentRoutes(svr);
    services::registerFacultyRoutes(svr);

    // 4. Add a default redirect for the root URL
    svr.Get("/", [](const Request& req, Response& res) {
        res.set_redirect("/admin.html");
    });

    cout << "\n==========================================" << endl;
    cout << "Starting College Management Web Server..." << endl;
    cout << "Open your browser to: http://localhost:8080" << endl;
    cout << "-- Press Ctrl+C to Stop --" << endl;
    cout << "==========================================\n" << endl;

    // Start server on port 8080
    svr.listen("0.0.0.0", 8080);

    return 0;
}
