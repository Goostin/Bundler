#pragma once

#include <string>
#include <vector>
#include <istream>
#include <ostream>

#include "category.hpp"

struct Product
{
    std::string product_id;
    std::string product_name;
    Category Category;
    double cost{};
    double msrp{};
    std::string launch_date;
    bool discontinued{};
};

inline std::ostream& operator<<(std::ostream& os, const Product& product)
{
    os << "Product { "
       << "id: " << product.product_id << ", "
       << "name: " << product.product_name << ", "
       << "category: " << product.Category << ", "
       << "cost: $" << product.cost << ", "
       << "msrp: $" << product.msrp << ", "
       << "launch_date: " << product.launch_date << ", "
       << "discontinued: " << (product.discontinued ? "true" : "false")
       << " }";

    return os;
}

std::vector<Product> parse_products(std::istream& file);
