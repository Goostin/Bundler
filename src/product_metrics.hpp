#pragma once

#include <functional>
#include <optional>

#include "purchase.hpp"
#include "product.hpp"

template<typename T>
using ProductMetric = std::function<T(const std::vector<Purchase>&, std::string_view)>;

struct ProductMetrics
{
    std::string product_id;
    Quantity units_sold;
    Quantity units_returned;
    double gross_revenue;
    double discount_amount;
    double refund_amount;
    std::optional<double> avg_rating;
};

Quantity calculate_units_sold(
    const std::vector<Purchase>& purchases,
    std::string_view product_id);

Quantity calculate_units_returned(
    const std::vector<Purchase>& purchases,
    std::string_view product_id);

double calculate_gross_revenue(
    const std::vector<Purchase>& purchases,
    std::string_view product_id);

double calculate_discount_amount(
    const std::vector<Purchase>& purchases,
    std::string_view product_id);

double calculate_refund_amount(
    const std::vector<Purchase>& purchases,
    std::string_view product_id);

std::optional<double> calculate_avg_rating(
    const std::vector<Purchase>& purchases,
    std::string_view product_id);

ProductMetrics calculate_product_metrics(
    const Product& product,
    const std::vector<Purchase>& purchases);
