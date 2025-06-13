#include <local_search/Relocation.h>
#include <limits>

namespace cvrp {

static constexpr double EPS  = 1e-4;   // ignore <0.0001 deltas
static constexpr int    MAX_PASSES = 2000;

bool Relocation::apply(Solution& sol, const Instance& inst)
{
    static int globalPass = 0;          // guards against infinite loops
    if (++globalPass > MAX_PASSES) return false;

    const int depot = inst.depot();
    auto& routes    = sol.routes();

    double bestDelta = -EPS;            // need strictly better than –EPS
    size_t fromR = 0, fromPos = 0;
    size_t toR   = 0, toPos   = 0;

    // ── 1) search best improving move ────────────────────────────────────
    for (size_t rIdx = 0; rIdx < routes.size(); ++rIdx) {
        auto& R1 = routes[rIdx];

        for (size_t pos = 0; pos < R1.customers.size(); ++pos) {
            int v        = R1.customers[pos];
            int demand_v = inst.demands()[v - 1];

            int prev = (pos == 0) ? depot : R1.customers[pos - 1];
            int next = (pos + 1 == R1.customers.size()) ? depot
                                                        : R1.customers[pos + 1];
            double remove = -inst.dist(prev, v)
                            -inst.dist(v, next)
                            +inst.dist(prev, next);

            for (size_t r2 = 0; r2 < routes.size(); ++r2) {
                auto& R2 = routes[r2];
                if (rIdx != r2 && R2.load + demand_v > inst.capacity()) continue;

                for (size_t p2 = 0; p2 <= R2.customers.size(); ++p2) {
                    // same-route, same position → skip
                    if (rIdx == r2 && (p2 == pos || p2 == pos + 1)) continue;

                    int before = (p2 == 0) ? depot : R2.customers[p2 - 1];
                    int after  = (p2 == R2.customers.size()) ? depot
                                                             : R2.customers[p2];
                    double add = inst.dist(before, v)
                               + inst.dist(v,      after)
                               - inst.dist(before, after);

                    double delta = remove + add;
                    if (delta < bestDelta) {
                        bestDelta = delta;
                        fromR = rIdx; fromPos = pos;
                        toR   = r2;   toPos   = p2;
                    }
                }
            }
        }
    }

    // ── 2) apply move ───────────────────────────────────────────────────
    if (bestDelta < -EPS) {
        auto& Rfrom = routes[fromR];
        int v = Rfrom.customers[fromPos];
        int demand_v = inst.demands()[v - 1];

        // erase
        int prev = (fromPos == 0) ? depot : Rfrom.customers[fromPos - 1];
        int next = (fromPos + 1 == Rfrom.customers.size()) ? depot
                    : Rfrom.customers[fromPos + 1];
        Rfrom.length += -inst.dist(prev, v) - inst.dist(v, next) + inst.dist(prev, next);
        Rfrom.customers.erase(Rfrom.customers.begin() + fromPos);
        Rfrom.load -= demand_v;

        // adjust insertion index if same route & after original pos
        if (fromR == toR && toPos > fromPos) --toPos;

        // insert
        auto& Rto = routes[toR];
        Rto.customers.insert(Rto.customers.begin() + toPos, v);
        int before = (toPos == 0) ? depot : Rto.customers[toPos - 1];
        int after  = (toPos + 1 == Rto.customers.size()) ? depot
                       : Rto.customers[toPos + 1];
        Rto.length += inst.dist(before, v)
                    + inst.dist(v, after)
                    - inst.dist(before, after);
        Rto.load += demand_v;

        sol.setTotalCost(sol.totalCost() + bestDelta);
        return true;
    }
    return false;   // no improving relocation
}

} // namespace cvrp
