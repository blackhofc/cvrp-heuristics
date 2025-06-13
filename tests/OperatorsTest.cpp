#include <gtest/gtest.h>

#include <core/Instance.h>
#include <solver/Solver.h>
#include <heuristics/ClarkeWrightSavings.h>
#include <local_search/Relocation.h>
#include <local_search/Swap.h>
#include <local_search/TwoOpt.h>

static const char* instPath = "data/E016-03m.dat";   // adjust if different

// helper: returns true if every customer appears exactly once
static bool customersOnce(const cvrp::Instance& inst,
                          const cvrp::Solution& sol)
{
    std::vector<int> freq(inst.dimension() + 1, 0);
    for (auto& r : sol.routes())
        for (int c : r.customers) ++freq[c];
    for (int i = 2; i <= inst.dimension(); ++i)
        if (freq[i] != 1) return false;
    return true;
}

TEST(Operators, SafetyAfterLS)
{
    cvrp::Instance inst(instPath);

    cvrp::Solver solver(std::make_unique<cvrp::ClarkeWrightSavings>());
    solver.addOperator(std::make_unique<cvrp::Relocation>());
    solver.addOperator(std::make_unique<cvrp::Swap>());
    solver.addOperator(std::make_unique<cvrp::TwoOpt>());

    cvrp::Solution initial = solver.run(inst);              // LS already inside
    EXPECT_TRUE(customersOnce(inst, initial));

    // capacity check
    for (auto& r : initial.routes())
        EXPECT_LE(r.load, inst.capacity());
}
