#pragma once

#include <cstdint>
#include <string>
#include <ostream>
#include <string_view>

#include "file_utilities.hpp"

enum class Category : uint8_t
{
    None,
    Kitchen,
    Outdoors,
    Care,
    Stationary,
    Home,
    Food,
    Apparel,
    Electronics
};

inline Category from_string(const std::string& category_str)
{
    const auto lower_category = to_lower(category_str);
    
    if (lower_category == "kitchen") return Category::Kitchen;
    if (lower_category == "outdoors") return Category::Outdoors;
    if (lower_category == "personal care") return Category::Care;
    if (lower_category == "stationery") return Category::Stationary;
    if (lower_category == "home") return Category::Home;
    if (lower_category == "food") return Category::Food;
    if (lower_category == "apparel") return Category::Apparel;
    if (lower_category == "electronics") return Category::Electronics;
    throw std::runtime_error("Unknown category: " + category_str);
}

inline std::string_view to_string(Category c)
{
    switch (c)
    {
        case Category::None: return "None";
        case Category::Kitchen: return "Kitchen";
        case Category::Outdoors: return "Outdoors";
        case Category::Care: return "Personal Care";
        case Category::Stationary: return "Stationery";
        case Category::Home: return "Home";
        case Category::Food: return "Food";
        case Category::Apparel: return "Apparel";
        case Category::Electronics: return "Electronics";
    }
    return "Unknown";
}

inline std::ostream& operator<<(std::ostream& os, Category c)
{
    return os << to_string(c);
}
