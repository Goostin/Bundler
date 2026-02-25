#pragma once

#include <string>
#include <vector>
#include <optional>
#include <istream>
#include <ostream>

using Quantity = std::int32_t;
using Rating = std::uint8_t;
using OptRating = std::optional<Rating>;

struct Purchase
{
    std::string product_id;
    Quantity quantity_purchased;
    Quantity quantity_returned;
    double unit_price;
    double discount_rate;
    OptRating rating;
};

inline std::ostream& operator<<(std::ostream& os, const Purchase& purchase)
{
    os << "Purchase { "
       << "product_id: " << purchase.product_id << ", "
       << "quantity_purchased: " << purchase.quantity_purchased << ", "
       << "quantity_returned: " << purchase.quantity_returned << ", "
       << "unit_price: $" << purchase.unit_price << ", "
       << "discount: " << purchase.discount_rate << ", "
       << "rating: ";

    if (purchase.rating.has_value())
    {
        os << static_cast<int>(purchase.rating.value());
    }
    else
    {
        os << "none";
    }
    os << " }";

    return os;
}

std::vector<Purchase> parse_order_items(std::istream& file);
