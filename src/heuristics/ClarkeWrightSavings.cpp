#include <heuristics/ClarkeWrightSavings.h>

#include <algorithm>
#include <vector>

namespace cvrp {

/* ----------------------------------------------------------------------------
   Clarke & Wright Savings Heuristic (parallel version, both orientations)
   ----------------------------------------------------------------------------
   1.  Compute savings s(i,j) = d0i + d0j − dij for all customer pairs.
   2.  Sort savings descending.
   3.  Begin with one route per customer (depot‑i‑depot).
   4.  Scan savings list and merge routes whenever possible, considering **both
       valid orientations**:
         · tail(i) → head(j)   (append Rj to the end of Ri)
         · tail(j) → head(i)   (append Ri to the end of Rj)  [handled after swap]
         · head(i) ← tail(j)   (prepend Rj in front of Ri)
         · head(j) ← tail(i)   (prepend Ri in front of Rj)  [handled after swap]
       A merge is feasible if routes are distinct and the combined load ≤ C.
   5.  Pack surviving routes into a Solution.
   -------------------------------------------------------------------------*/

Solution ClarkeWrightSavings::build(const Instance& inst)
{
    const int depot = inst.depot();                 // usually 1 in VRPLIB files
    const int n     = inst.dimension() - 1;         // number of customers

    // ------------------------------------------------------------------
    // 1) Compute savings list
    // ------------------------------------------------------------------
    struct Saving { int i, j; double val; };
    std::vector<Saving> savings;
    savings.reserve(n * (n - 1) / 2);

    for (int i = 2; i <= n + 1; ++i)
        for (int j = i + 1; j <= n + 1; ++j)
            savings.push_back({i, j,
                               inst.dist(depot, i) + inst.dist(depot, j) - inst.dist(i, j)});

    std::sort(savings.begin(), savings.end(),
              [](const Saving& a, const Saving& b) { return a.val > b.val; });

    // ------------------------------------------------------------------
    // 2) Create one singleton route per customer
    // ------------------------------------------------------------------
    const auto& demand = inst.demands();
    std::vector<Route> routes;              // stable storage (pointers stay valid)
    routes.reserve(n);
    std::vector<Route*> whichRoute(inst.dimension() + 1, nullptr); // node id → route*

    for (int cid = 2; cid <= n + 1; ++cid) {
        Route r;
        r.customers.push_back(cid);
        r.load   = demand[cid - 1];               // demands are 0‑based in VRPLIBReader
        r.length = 2.0 * inst.dist(depot, cid);   // depot‑cid‑depot

        routes.push_back(std::move(r));
        whichRoute[cid] = &routes.back();
    }

    // ------------------------------------------------------------------
    // 3) Merge routes following savings (parallel, both orientations)
    // ------------------------------------------------------------------
    auto rewire = [&](Route* from, Route* to) {
        for (int c : from->customers) whichRoute[c] = to;
        from->customers.clear();
    };

    for (const auto& s : savings) {
        Route* Ri = whichRoute[s.i];
        Route* Rj = whichRoute[s.j];
        if (!Ri || !Rj || Ri == Rj) continue;

        // -------- Orientation 1: tail(i) -> head(j) ------------------
        if (Ri->tail() == s.i && Rj->head() == s.j) {
            if (Ri->load + Rj->load > inst.capacity()) continue;

            double delta = Rj->length
                           - inst.dist(depot, Ri->tail())
                           - inst.dist(depot, Rj->head())
                           + inst.dist(Ri->tail(), Rj->head());

            Ri->length += delta;
            Ri->load   += Rj->load;
            Ri->customers.insert(Ri->customers.end(),
                                 Rj->customers.begin(), Rj->customers.end());
            rewire(Rj, Ri);
            continue;
        }

        // -------- Orientation 2: tail(j) -> head(i)  (swap roles) -----
        if (Rj->tail() == s.j && Ri->head() == s.i) {
            if (Ri->load + Rj->load > inst.capacity()) continue;

            double delta = Rj->length
                           - inst.dist(depot, Rj->tail())
                           - inst.dist(depot, Ri->head())
                           + inst.dist(Rj->tail(), Ri->head());

            Ri->length += delta;
            Ri->load   += Rj->load;
            // prepend Rj in front of Ri
            Ri->customers.insert(Ri->customers.begin(),
                                 Rj->customers.begin(), Rj->customers.end());
            rewire(Rj, Ri);
            continue;
        }
    }

    // ------------------------------------------------------------------
    // 4) Pack surviving routes
    // ------------------------------------------------------------------
    Solution sol;
    for (Route& r : routes)
        if (!r.customers.empty())
            sol.addRoute(std::move(r));

    return sol;
}

} // namespace cvrp
