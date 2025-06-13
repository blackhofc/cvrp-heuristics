#pragma once
#include <core/Instance.h>
#include <core/Solution.h>

namespace cvrp {

class LSOperator {
public:
    virtual ~LSOperator() = default;
    /// Aplica el operador una vez; true si la solución mejora.
    virtual bool apply(Solution& sol, const Instance& inst) = 0;
};

} // namespace cvrp
