/// @file proof_reduce.cpp
///
/// Proves that the lambdas used with std::reduce in product_metrics.cpp
/// are not suitable for std::reduce.
///
/// std::reduce, unlike std::accumulate, may call binary_op with ANY
/// combination of its intermediate value types.  With init=double and a
/// range of Purchase objects, a conforming implementation must be able to
/// call:
///
///   op(double,   Purchase) -- adding one element to a running total   ✓ works
///   op(Purchase, double)   -- commuted form of the above              ✗ FAILS
///   op(double,   double)   -- merging two partial sub-totals          ✗ FAILS
///   op(Purchase, Purchase) -- combining two elements before the init  ✗ FAILS
///
/// GCC 13's libstdc++ enforces this directly inside std::reduce via its
/// own static_assert checks (see <numeric> lines 292-294 in GCC 13):
///
///   static_assert(is_invocable_r_v<_Tp, _BinaryOp&, __ref, _Tp&>);   // (Purchase, double)
///   static_assert(is_invocable_r_v<_Tp, _BinaryOp&, _Tp&,  _Tp&>);   // (double, double)
///   static_assert(is_invocable_r_v<_Tp, _BinaryOp&, __ref, __ref>);  // (Purchase, Purchase)
///
/// Attempting to compile std::reduce with the lambda below therefore
/// produces three static_assert errors from inside the standard library
/// itself — the ultimate proof.
///
/// This file demonstrates the same requirements via our own static_asserts
/// and shows that std::accumulate, which only ever calls op(double, Purchase)
/// left-to-right, is the correct tool.

#include <cstdint>
#include <iostream>
#include <numeric>
#include <optional>
#include <string>
#include <type_traits>
#include <vector>

/// Minimal local reproduction of Purchase so this file compiles
/// independently of the project headers (which have their own bugs).
struct Purchase
{
    std::string  product_id;
    std::int32_t quantity_purchased{};
    std::int32_t quantity_returned{};
    double       unit_price{};
    double       discount_rate{};
    std::optional<std::uint8_t> rating;
};

int main()
{
    // -----------------------------------------------------------------------
    // The binary_op used in calculate_gross_revenue — representative of all
    // the lambdas in product_metrics.cpp that follow the same pattern.
    // -----------------------------------------------------------------------
    const std::string product_id = "P001";

    auto op = [&product_id](double sum, const Purchase& p) -> double
    {
        return (p.product_id == product_id)
            ? sum + p.quantity_purchased * p.unit_price
            : sum;
    };

    // -----------------------------------------------------------------------
    // Compile-time proof: mirror the exact checks GCC 13 performs inside
    // std::reduce (<numeric> lines 292-294).  Each failing case below
    // corresponds to one of the three static_assert errors emitted by the
    // standard library when the call to std::reduce is attempted.
    // -----------------------------------------------------------------------

    // The only combination the lambda supports (left-fold order):
    static_assert( std::is_invocable_r_v<double, decltype(op), double,   Purchase>,
        "op(double, Purchase) -- the only case the lambda handles");

    // GCC check 1 — commuted element/accumulator order:
    static_assert(!std::is_invocable_r_v<double, decltype(op), Purchase, double>,
        "PROOF: op(Purchase, double) is NOT callable"
        " -- required by std::reduce, fails here");

    // GCC check 2 — merging two partial double sub-totals:
    static_assert(!std::is_invocable_r_v<double, decltype(op), double,   double>,
        "PROOF: op(double, double) is NOT callable"
        " -- required by std::reduce to merge parallel sub-totals");

    // GCC check 3 — combining two adjacent range elements before the init:
    static_assert(!std::is_invocable_r_v<double, decltype(op), Purchase, Purchase>,
        "PROOF: op(Purchase, Purchase) is NOT callable"
        " -- required by std::reduce to combine adjacent elements");

    // -----------------------------------------------------------------------
    // Runtime demonstration: std::accumulate is correct here.
    // std::accumulate only ever calls op(double, Purchase) left-to-right.
    // -----------------------------------------------------------------------
    const std::vector<Purchase> purchases = {
        {"P001", 2, 0, 10.0, 0.0, std::nullopt},   // contributes 20.0
        {"P002", 5, 0,  3.0, 0.0, std::nullopt},   // should be filtered out
        {"P001", 3, 0,  5.0, 0.0, std::nullopt},   // contributes 15.0
    };
    const double expected = 35.0; // 2*10 + 3*5

    const double result =
        std::accumulate(purchases.begin(), purchases.end(), 0.0, op);

    std::cout << "std::accumulate result : " << result
              << "  (expected: " << expected << ")\n\n";

    std::cout
        << "The three static_asserts above prove the binary_op cannot\n"
        << "satisfy std::reduce's requirements.  It is only callable as:\n"
        << "  op(double,   Purchase) ✓\n"
        << "and NOT callable as any of:\n"
        << "  op(Purchase, double)   ✗  (commuted element/accumulator)\n"
        << "  op(double,   double)   ✗  (merging two partial sub-totals)\n"
        << "  op(Purchase, Purchase) ✗  (combining two adjacent elements)\n\n"
        << "GCC 13's std::reduce enforces these same three checks with its own\n"
        << "static_asserts (see <numeric>:292-294), so attempting to compile\n"
        << "std::reduce with this lambda is an immediate compile error on GCC 13.\n"
        << "std::accumulate, which only ever calls op(double, Purchase) in order,\n"
        << "is the correct algorithm for this operation.\n";

    return (result == expected) ? 0 : 1;
}
