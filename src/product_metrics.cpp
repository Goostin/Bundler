#include "product_metrics.hpp"

#include <execution>
#include <numeric>
#include <optional>

int calculate_units_sold(
    const std::vector<Purchase>& purchases,
    std::string_view product_id)
{
    return std::reduce(
        purchases.begin(), purchases.end(),
        0, 
        [product_id](int sum, const Purchase& purchase){
            return ( purchase.product_id == product_id )
                ? sum + purchase.quantity_purchased
                : sum;
        }
    );
}

int calculate_units_returned(
    const std::vector<Purchase>& purchases,
    std::string_view product_id)
{
    return std::reduce(
        purchases.begin(), purchases.end(),
        0, 
        [product_id](int sum, const Purchase& purchase){
            return ( purchase.product_id == product_id )
                ? sum + purchase.quantity_returned
                : sum;
        }
    );
}

double calculate_gross_revenue(
    const std::vector<Purchase>& purchases,
    std::string_view product_id)
{
    return std::reduce(
        purchases.begin(), purchases.end(),
        0.0, 
        [product_id](double sum, const Purchase& purchase){ 
            return ( purchase.product_id == product_id )
                ? sum + purchase.quantity_purchased * purchase.unit_price
                : sum; 
        }
    );
}

double calculate_discount_amount(
    const std::vector<Purchase>& purchases,
    std::string_view product_id)
{
    return std::reduce(
        purchases.begin(), purchases.end(),
        0.0, 
        [product_id](double sum, const Purchase& purchase){
            return ( purchase.product_id == product_id )
                ? sum + purchase.quantity_purchased
                        * purchase.unit_price
                        * purchase.discount_rate
                : sum;
        }
    );
}

double calculate_refund_amount(
    const std::vector<Purchase>& purchases,
    std::string_view product_id)
{  
    return std::reduce(
        purchases.begin(), purchases.end(),
        0.0, 
        [product_id](double sum, const Purchase& purchase){
            return ( purchase.product_id == product_id )
                ? sum + purchase.quantity_returned
                        * purchase.unit_price
                        * ( 1.0 - purchase.discount_rate )
                : sum;
        }
    );
}

std::optional<double> calculate_avg_rating(
    const std::vector<Purchase>& purchases,
    std::string_view product_id)
{
    auto [sum, count] = std::reduce(
        purchases.begin(), purchases.end(),
        std::pair{0.0, std::size_t{0}},
        [product_id](auto sum, const Purchase& purchase){
            return ( purchase.product_id == product_id && purchase.rating.has_value() )
                ? std::pair{sum.first + *purchase.rating, sum.second + 1}
                : sum;
        }
    );
    return (count == 0.0) ? std::nullopt : std::optional<double>(sum / count);
}

ProductMetrics calculate_product_metrics(
    const Product &product,
    const std::vector<Purchase> &purchases)
{
    return ProductMetrics
    {
        .product_id = product.product_id,
        .units_sold = calculate_units_sold(purchases, product.product_id),
        .units_returned = calculate_units_returned(purchases, product.product_id),
        .gross_revenue = calculate_gross_revenue(purchases, product.product_id),
        .discount_amount = calculate_discount_amount(purchases, product.product_id),
        .refund_amount = calculate_refund_amount(purchases, product.product_id),
        .avg_rating = calculate_avg_rating(purchases, product.product_id)
    };
}
