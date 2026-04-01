#pragma once
#include <string>
#include <cctype>

// --- Helper Functions for simple JSON parsing ---
// Note: Hand-rolled string finders to avoid heavy dependencies on older compilers
namespace utils {

    inline std::string get_json_string(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\":\"";
        size_t start = json.find(searchKey);
        if (start == std::string::npos) return "";
        start += searchKey.length();
        size_t end = json.find("\"", start);
        return json.substr(start, end - start);
    }

    inline std::string get_json_number(const std::string& json, const std::string& key) {
        std::string searchKey = "\"" + key + "\":";
        size_t start = json.find(searchKey);
        if (start == std::string::npos) return "0";
        start += searchKey.length();
        size_t end = start;
        while(end < json.length() && (isdigit(json[end]) || json[end]=='.' || json[end]=='-')) end++;
        return json.substr(start, end - start);
    }

}
