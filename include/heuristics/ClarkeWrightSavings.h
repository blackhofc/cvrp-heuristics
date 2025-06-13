#pragma once
#include <heuristics/Heuristic.h>
#include <vector>

namespace cvrp {

class ClarkeWrightSavings final : public Heuristic {
public:
    explicit ClarkeWrightSavings(bool parallel = true)
        : _parallel(parallel) {}

    Solution build(const Instance&) override;

private:
    bool _parallel;   // parallel vs sequential version
};

} // namespace cvrp
