#include <solver/Solver.h>

namespace cvrp {

//-------------------------------------
// constructor
//-------------------------------------
Solver::Solver(std::unique_ptr<Heuristic> h)
    : _heuristic(std::move(h)) {}

//-------------------------------------
// register a local-search operator
//-------------------------------------
void Solver::addOperator(std::unique_ptr<LSOperator> op)
{
    _ops.push_back(std::move(op));
}

//-------------------------------------
// run: heuristic  ➜  local search loop
//-------------------------------------
Solution Solver::run(const Instance& inst)
{
    // 1) build an initial solution
    Solution sol = _heuristic->build(inst);

    // 2) apply local-search operators until none improves (first-improvement)
    bool improved = true;
    while (improved) {
        improved = false;
        for (auto& op : _ops) {
            if (op->apply(sol, inst)) {
                improved = true;   // solution got better
                break;             // restart with first operator
            }
        }
    }
    return sol;
}

} // namespace cvrp
