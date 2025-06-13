#include <cassert>
#include <core/Instance.h>
#include <solver/Solver.h>
#include <heuristics/ClarkeWrightSavings.h>
#include <local_search/Relocation.h>
#include <local_search/Swap.h>
#include <local_search/TwoOpt.h>

static const char* path = "data/E016-03m.dat";   // small public instance

// helper: every customer appears exactly once
static bool unique_customers(const cvrp::Instance& inst,
                             const cvrp::Solution& sol)
{
    std::vector<int> seen(inst.dimension() + 1, 0);
    for (auto& r : sol.routes())
        for (int c : r.customers) ++seen[c];
    for (int id = 2; id <= inst.dimension(); ++id)
        if (seen[id] != 1) return false;
    return true;
}

int main() {
    cvrp::Instance inst(path);

    cvrp::Solver solver(std::make_unique<cvrp::ClarkeWrightSavings>());
    solver.addOperator(std::make_unique<cvrp::Relocation>());
    solver.addOperator(std::make_unique<cvrp::Swap>());
    solver.addOperator(std::make_unique<cvrp::TwoOpt>());

    cvrp::Solution sol = solver.run(inst);

    // no capacity violation
    for (auto& r : sol.routes())
        assert(r.load <= inst.capacity());

    // every customer served exactly once
    assert(unique_customers(inst, sol));

    return 0;
}
