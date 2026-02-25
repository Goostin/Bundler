#include "bundling.hpp"

#include <algorithm>
#include <numeric>
#include <optional>

#include "derived_product_metrics.hpp"
#include "product_metrics.hpp"

namespace {

auto ineligible_product_predicate = [](const ProductMetrics& metrics){
    constexpr double min_rating = 4.0;
    constexpr double max_return_rate = 0.2;
    constexpr double min_net_revenue = 0.0;

    if (metrics.avg_rating.has_value() && metrics.avg_rating.value() < min_rating)
    {
        return true;
    }

    if (metrics.units_sold > 0 
        && (static_cast<double>(metrics.units_returned) / metrics.units_sold) > max_return_rate)
    {
        return true;
    }

    if ((metrics.gross_revenue - metrics.discount_amount - metrics.refund_amount) < min_net_revenue)
    {
        return true;
    }

    return false;
};

double product_score(
    double normalized_net_revenue,
    double normalized_return_rate,
    double normalized_rating_score)
{
    constexpr double net_revenue_weight = 0.55;
    constexpr double rating_score_weight = 0.25;
    constexpr double return_rate_weight = 0.2;

    return net_revenue_weight * normalized_net_revenue +
           rating_score_weight * normalized_rating_score + 
           return_rate_weight * (1.0 - normalized_return_rate);
}

template <typename T>
T normalize(T value, T max_value, T min_value)
{
    return (max_value > min_value) ? (value - min_value) / (max_value - min_value) : 0.0;
}

double product_score(
    const ProductMetrics& metrics,
    std::pair<double, double> net_revenue_range,
    std::pair<double, double> return_rate_range,
    std::pair<double, double> rating_score_range)
{
    auto normalized_net_revenue =
        normalize(net_revenue(metrics), net_revenue_range.second, net_revenue_range.first);

    auto normalized_return_rate =
        normalize(return_rate(metrics), return_rate_range.second, return_rate_range.first);

    auto normalized_rating_score =
        normalize(rating_score(metrics), rating_score_range.second, rating_score_range.first);

    return product_score(normalized_net_revenue, normalized_return_rate, normalized_rating_score);
}

bool break_tie(const ProductMetrics& m1, const ProductMetrics& m2)
{
    if (net_revenue(m1) != net_revenue(m2))
    {
        return (net_revenue(m1) > net_revenue(m2));
    }

    if (return_rate(m1) != return_rate(m2))
    {
        return (return_rate(m1) < return_rate(m2));
    }

    if (rating_score(m1) != rating_score(m2))
    {
        return (rating_score(m1) > rating_score(m2));
    }

    return (m1.product_id < m2.product_id);
}

} // End of namespace

void filter_ineligible(std::vector<ProductMetrics>& metrics)
{
    metrics.erase(
        std::remove_if(metrics.begin(), metrics.end(), ineligible_product_predicate),
        metrics.end()
    );
}

void sort_by_score(std::vector<ProductMetrics>& metrics)
{
    auto revenue_range = derived_metric_min_max<double>(
        metrics,
        [](const ProductMetrics& m) { return net_revenue(m); }
    );

    auto return_rate_range = derived_metric_min_max<double>(
        metrics,
        [](const ProductMetrics& m) { return return_rate(m); }
    );

    auto rating_score_range = derived_metric_min_max<double>(
        metrics,
        [](const ProductMetrics& m) { return rating_score(m); }
    );

    std::sort(
        metrics.begin(), metrics.end(),
        [&revenue_range, &return_rate_range, &rating_score_range](
            const ProductMetrics& a,
            const ProductMetrics& b)
        {
            /// @todo - These are reasonably cheap to compute, but we could precompute / cache these.
            double score_a = product_score(a, revenue_range, return_rate_range, rating_score_range);
            double score_b = product_score(b, revenue_range, return_rate_range, rating_score_range);

            if (score_a == score_b)
            {
                return break_tie(a, b);
            }
            return score_a > score_b;
        }
    );
}

std::vector<Product> recommend_bundle(
    const std::vector<Product> &products,
    const std::vector<ProductMetrics> &metrics)
{
    std::vector<Product> bundle;
    std::vector<Category> included_categories;

    for (const auto& metric : metrics)
    {
        auto product_it = std::find_if(
            products.begin(), products.end(),
            [&metric](const Product& p) { return p.product_id == metric.product_id; }
        );

        if (product_it == products.end())
        {
            continue;
        }

        const auto begin = included_categories.begin();
        const auto end = included_categories.end();

        if (std::find(begin, end, product_it->Category) == end)
        {
            included_categories.push_back(product_it->Category);
            bundle.push_back(*product_it);
        }
        else if (( BUNDLE_SIZE - bundle.size() ) > ( MIN_NUM_CATEGORIES - included_categories.size() ))
        {
            bundle.push_back(*product_it);
        }

        if (bundle.size() >= BUNDLE_SIZE)
        {
            return bundle;
        }
    }
    return std::vector<Product>{};
}
