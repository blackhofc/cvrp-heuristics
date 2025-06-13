#pragma once
#include <core/Instance.h>
#include <core/Solution.h>

namespace cvrp {

class LSOperator {
public:
    virtual ~LSOperator() = default;
    /// Applies the operator one time; true if solution its better.
    virtual bool apply(Solution& sol, const Instance& inst) = 0;
};

} // namespace cvrp
