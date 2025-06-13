#include <local_search/Swap.h>

namespace cvrp {

static constexpr double EPS        = 1e-4;   // minimum real improvement
static constexpr int    MAX_PASSES = 5000;   // absolute safety net

bool Swap::apply(Solution& sol, const Instance& inst)
{
    static int globalPass = 0;
    if (++globalPass > MAX_PASSES) return false;

    const int depot = inst.depot();
    auto& routes    = sol.routes();

    // -------------------------------------------------------------
    // Search first improving swap (you can change to steepest later)
    // -------------------------------------------------------------
    for (size_t rA = 0; rA < routes.size(); ++rA) {
        auto& A = routes[rA];

        for (size_t i = 0; i < A.customers.size(); ++i) {
            int v  = A.customers[i];
            int dv = inst.demands()[v - 1];

            int Aprev = (i == 0) ? depot : A.customers[i - 1];
            int Anext = (i + 1 == A.customers.size()) ? depot
                                                      : A.customers[i + 1];

            for (size_t rB = rA; rB < routes.size(); ++rB) {
                auto& B = routes[rB];
                size_t jStart = (rA == rB) ? i + 1 : 0;   // avoid dup pairs

                for (size_t j = jStart; j < B.customers.size(); ++j) {
                    int w  = B.customers[j];
                    int dw = inst.demands()[w - 1];

                    // capacity check if routes differ
                    if (rA != rB) {
                        if (A.load - dv + dw > inst.capacity()) continue;
                        if (B.load - dw + dv > inst.capacity()) continue;
                    }

                    int Bprev = (j == 0) ? depot : B.customers[j - 1];
                    int Bnext = (j + 1 == B.customers.size()) ? depot
                                                              : B.customers[j + 1];

                    double delta;

                    if (rA == rB) {
                        // -------- intra-route swap (only route A changes) ------
                        // Special cases when nodes are adjacent need care.
                        if (j == i + 1) {
                            // ... Aprev - v - w - Anext ...
                            delta =  inst.dist(Aprev, w) + inst.dist(w, v) + inst.dist(v, Anext)
                                   - inst.dist(Aprev, v) - inst.dist(v, w) - inst.dist(w, Anext);
                        } else {
                            // non-adjacent inside same route
                            delta =  inst.dist(Aprev, w) + inst.dist(w, Anext)
                                   + inst.dist(Bprev, v) + inst.dist(v, Bnext)
                                   - inst.dist(Aprev, v) - inst.dist(v, Anext)
                                   - inst.dist(Bprev, w) - inst.dist(w, Bnext);
                        }

                        if (delta < -EPS) {
                            std::swap(A.customers[i], A.customers[j]);
                            A.length += delta;
                            sol.setTotalCost(sol.totalCost() + delta);
                            return true;
                        }
                    } else {
                        // -------- inter-route swap ----------------------------
                        double oldCost =  inst.dist(Aprev, v) + inst.dist(v, Anext)
                                        + inst.dist(Bprev, w) + inst.dist(w, Bnext);

                        double newCost =  inst.dist(Aprev, w) + inst.dist(w, Anext)
                                        + inst.dist(Bprev, v) + inst.dist(v, Bnext);

                        delta = newCost - oldCost;
                        if (delta < -EPS) {
                            // execute swap
                            std::swap(A.customers[i], B.customers[j]);
                            A.length += delta;
                            B.length += delta;      // same magnitude
                            A.load = A.load - dv + dw;
                            B.load = B.load - dw + dv;
                            sol.setTotalCost(sol.totalCost() + delta);
                            return true;
                        }
                    }
                }
            }
        }
    }
    return false;   // no improving swap found
}

} // namespace cvrp
