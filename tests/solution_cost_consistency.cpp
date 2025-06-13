#include <cassert>
#include <core/Instance.h>
#include <solver/Solver.h>
#include <heuristics/ClarkeWrightSavings.h>
#include <local_search/Relocation.h>
#include <local_search/Swap.h>
#include <local_search/TwoOpt.h>

static const char* path = "data/E016-03m.dat";   // any small instance

int main()
{
    // 1) build a full solution (heuristic + LS)
    cvrp::Instance inst(path);

    cvrp::Solver solver(std::make_unique<cvrp::ClarkeWrightSavings>());
    solver.addOperator(std::make_unique<cvrp::Relocation>());
    solver.addOperator(std::make_unique<cvrp::Swap>());
    solver.addOperator(std::make_unique<cvrp::TwoOpt>());

    cvrp::Solution sol = solver.run(inst);

    // 2) recompute total from route lengths
    double sum = 0.0;
    for (const auto& r : sol.routes()) sum += r.length;

    // 3) check consistency (tolerance 1e-6)
    assert(std::abs(sum - sol.totalCost()) < 1e-6);

    return 0;   // PASS
}
