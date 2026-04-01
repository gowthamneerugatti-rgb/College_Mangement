#pragma once
#include <string>

namespace utils {

    inline bool isValidEmail(const std::string& email) {
        return email.find("@") != std::string::npos && email.find(".") != std::string::npos;
    }

}
