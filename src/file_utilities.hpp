#pragma once

#include <algorithm>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

inline std::vector<std::string> split_csv_line(const std::string& line)
{
    std::vector<std::string> fields;
    std::stringstream ss(line);
    std::string field;
    
    while (std::getline(ss, field, ','))
    {
        fields.push_back(field);
    }
    
    return fields;
}

inline std::string to_lower(const std::string& str)
{
    std::string result = str;
    std::transform(
        result.begin(), result.end(),
        result.begin(),
        [](unsigned char c){ return std::tolower(c); });
    return result;
}

inline bool parse_bool(const std::string& bool_str)
{
    return to_lower(bool_str) == "true";
}
