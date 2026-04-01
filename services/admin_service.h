#pragma once
#include "../httplib.h"

namespace services {
    void registerAdminRoutes(httplib::Server& svr);
}
