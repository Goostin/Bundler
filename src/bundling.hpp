#pragma once

#include "derived_product_metrics.hpp"
#include "product_metrics.hpp"

constexpr std::size_t BUNDLE_SIZE = 3;
constexpr std::size_t MIN_NUM_CATEGORIES = 2;

void filter_ineligible(std::vector<ProductMetrics>& metrics);

void sort_by_score(std::vector<ProductMetrics>& metrics);

std::vector<Product> recommend_bundle(
    const std::vector<Product>& products,
    const std::vector<ProductMetrics>& metrics);
