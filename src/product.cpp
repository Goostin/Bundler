#include "product.hpp"

#include <stdexcept>

#include "category.hpp"
#include "file_utilities.hpp"

namespace {

Product make_product(const std::vector<std::string>& fields)
{
    return Product{
        .product_id = fields[0],
        .product_name = fields[1],
        .Category = from_string(fields[2]),
        .cost = std::stod(fields[3]),
        .msrp = std::stod(fields[4]),
        .launch_date = fields[5],
        .discontinued = parse_bool(fields[6])
    };
}

} // End of namespace

/// @todo: Make this a utility that takes build functions.
std::vector<Product> parse_products(std::istream& file)
{
    std::vector<Product> products;
    std::string line;
    
    // Skip header
    std::getline(file, line);
    
    while (std::getline(file, line))
    {
        auto fields = split_csv_line(line);
        if (fields.size() >= 7)
        {
            products.push_back(make_product(fields));
        }
    }
    
    return products;
}

