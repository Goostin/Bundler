#pragma once

#include <algorithm>
#include <numeric>
#include <optional>

#include "product_metrics.hpp"

template<typename T>
using DerivedProductMetric = std::function<T(const ProductMetrics&)>;

inline double net_revenue(const ProductMetrics& metrics)
{
    return metrics.gross_revenue - metrics.discount_amount - metrics.refund_amount;
}

inline double return_rate(const ProductMetrics& metrics)
{
    return (metrics.units_sold > 0)
        ? static_cast<double>(metrics.units_returned) / metrics.units_sold
        : 0.0;
}

inline double rating_score(const ProductMetrics& metrics)
{
    return metrics.avg_rating.value_or(0.0);
}

template <typename T, typename BinaryOperation>
T reduce_derived_metric(
    const std::vector<ProductMetrics>& metrics,
    DerivedProductMetric<T> metric_extractor,
    BinaryOperation op)
{
    return std::reduce(
        metrics.begin(), metrics.end(),
        T{},
        [op, metric_extractor](T reduced_value, const ProductMetrics& current) {
            return op(reduced_value, metric_extractor(current));
        }
    );
}

template <typename T>
std::pair<T, T> derived_metric_min_max(
    const std::vector<ProductMetrics>& metrics,
    DerivedProductMetric<T> metric_extractor)
{
    return std::make_pair(
        
        reduce_derived_metric<T>(
            metrics,
            metric_extractor,
            [](T a, T b) { return std::min(a, b); }),

        reduce_derived_metric<T>(
            metrics,
            metric_extractor,
            [](T a, T b) { return std::max(a, b); })
    );
}
