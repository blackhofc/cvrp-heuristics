#include <local_search/TwoOpt.h>

namespace cvrp {

static constexpr double EPS = 1e-4;   // minimal meaningful improvement

bool TwoOpt::apply(Solution& sol, const Instance& inst)
{
    const int depot = inst.depot();
    auto& routes    = sol.routes();

    for (auto& R : routes) {
        const size_t m = R.customers.size();
        if (m < 4) continue;                  // nothing to gain

        for (size_t i = 0; i < m - 2; ++i) {
            int a = (i == 0) ? depot : R.customers[i - 1];
            int b = R.customers[i];

            for (size_t j = i + 2; j < m; ++j) {
                int c = R.customers[j];
                int d = (j + 1 == m) ? depot : R.customers[j + 1];

                double delta =  inst.dist(a, c) + inst.dist(b, d)
                              - inst.dist(a, b) - inst.dist(c, d);
                if (delta < -EPS) {
                    // reverse the segment [i … j]
                    std::reverse(R.customers.begin() + i, R.customers.begin() + j + 1);
                    R.length += delta;
                    sol.setTotalCost(sol.totalCost() + delta);
                    return true;              // first-improvement: restart LS loop
                }
            }
        }
    }
    return false;                             // no improving 2-Opt found
}

} // namespace cvrp
