#pragma once
/**
 *  Solver — orchestrates a constructive heuristic + a list of local-search
 *  operators, applying them until no further improvement is found.
 *
 *  Typical usage:
 *      auto solver = cvrp::Solver(std::make_unique<cvrp::ClarkeWrightSavings>());
 *      solver.addOperator(std::make_unique<cvrp::Relocation>());
 *      solver.addOperator(std::make_unique<cvrp::Swap>());
 *      Solution sol = solver.run(instance);
 */

#include <memory>
#include <vector>

#include <core/Instance.h>
#include <core/Solution.h>
#include <heuristics/Heuristic.h>
#include <local_search/LSOperator.h>

namespace cvrp {

class Solver {
public:
    /// Construct with the heuristic that will build the initial solution.
    explicit Solver(std::unique_ptr<Heuristic> h);

    /// Register a local-search operator (the order they are added matters).
    void addOperator(std::unique_ptr<LSOperator> op);

    /// Run the whole pipeline and return the final solution.
    Solution run(const Instance& inst);

private:
    std::unique_ptr<Heuristic>               _heuristic;  // constructive phase
    std::vector<std::unique_ptr<LSOperator>> _ops;        // local-search ops
};

} // namespace cvrp
