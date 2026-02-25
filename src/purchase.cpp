#include "purchase.hpp"

#include "file_utilities.hpp"

namespace {

Purchase make_purchase(const std::vector<std::string>& fields)
{
    return Purchase
    {
        .product_id = fields[2],
        .quantity_purchased = std::stoi(fields[3]),
        .quantity_returned = std::stoi(fields[6]),
        .unit_price = std::stod(fields[4]),
        .discount_rate = std::stod(fields[5]),
        .rating = fields[8].empty() 
            ? std::nullopt 
            : OptRating(static_cast<Rating>(std::stoi(fields[8])))
    };
}

} // End of namespace

std::vector<Purchase> parse_order_items(std::istream& file)
{
    std::vector<Purchase> purchases;
    std::string line;
    
    // Skip header
    std::getline(file, line);
    
    while (std::getline(file, line))
    {
        auto fields = split_csv_line(line);

        if (fields.size() >= 9)
        {
            purchases.push_back(make_purchase(fields));
        }
    }
    
    return purchases;
}