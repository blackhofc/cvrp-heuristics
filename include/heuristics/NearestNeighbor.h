#pragma once
#include <heuristics/Heuristic.h>

namespace cvrp {

class NearestNeighbor final : public Heuristic {
public:
    Solution build(const Instance& inst) override;
};

} // namespace cvrp
