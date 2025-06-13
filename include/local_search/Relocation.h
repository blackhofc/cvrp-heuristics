#pragma once
#include <local_search/LSOperator.h>

namespace cvrp {

class Relocation final : public LSOperator {
public:
    bool apply(Solution& sol, const Instance& inst) override;
};

} // namespace cvrp
