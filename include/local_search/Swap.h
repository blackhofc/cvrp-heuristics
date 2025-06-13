#pragma once
#include <local_search/LSOperator.h>

namespace cvrp {

/**
 * Swap (1-1) — exchanges two customers; may be intra- or inter-route.
 * First-improvement: stops at the first cost-reducing swap.
 * Complexity:  O(r · n²)  where r = #routes, n = customers.
 */
class Swap final : public LSOperator {
public:
    bool apply(Solution& sol, const Instance& inst) override;
};

} // namespace cvrp
