#include <algorithm>
#include <fstream>
#include <iostream>

#include <bundling.hpp>
#include <purchase.hpp>
#include <product.hpp>
#include <product_metrics.hpp>

int main()
{
    auto products_file = std::ifstream("../data/products.csv");
    auto products = parse_products(products_file);

    auto ordered_items = std::ifstream("../data/order_items.csv");
    auto purchases = parse_order_items(ordered_items);

    std::vector<ProductMetrics> metrics;
    std::transform(
        products.begin(), products.end(),
        std::back_inserter(metrics),
        [&purchases](const Product& product){
            return calculate_product_metrics(product, purchases);
        }
    );

    filter_ineligible(metrics);
    sort_by_score(metrics);

    auto bundle = recommend_bundle(products, metrics);
    if (bundle.empty())
    {
        std::cerr << "Error: " 
                  << "Unable to create a complete bundle with the given constraints."
                  << std::endl;
                  
        return EXIT_FAILURE;
    }

    std::cout << "Recommended Bundle:" << std::endl;
    for (const auto& product : bundle)
    {
        std::cout << product << std::endl;
    }
    return EXIT_SUCCESS;
}