#pragma once
#include <algorithm>
#include <local_search/LSOperator.h>

namespace cvrp {

/**
 * In-route 2-Opt
 * Reverses a segment (i … j) inside a single route if doing so shortens the tour.
 * First-improvement: quits after the first improving reversal.
 * Complexity per apply(): O(n²) per route ⇒ O(r · n²) overall.
 */
class TwoOpt final : public LSOperator {
public:
    bool apply(Solution& sol, const Instance& inst) override;
};

} // namespace cvrp
