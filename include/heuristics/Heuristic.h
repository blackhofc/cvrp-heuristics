#pragma once
#include <core/Instance.h>
#include <core/Solution.h>

namespace cvrp {

class Heuristic {
public:
    virtual ~Heuristic() = default;
    /// Build an *initial* solution (may be infeasible wrt #vehicles upper-bound).
    virtual Solution build(const Instance&) = 0;
};

} // namespace cvrp
