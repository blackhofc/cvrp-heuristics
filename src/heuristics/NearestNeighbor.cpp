#include <heuristics/NearestNeighbor.h>
#include <limits>
#include <vector>

namespace cvrp {

Solution NearestNeighbor::build(const Instance& inst)
{
    const int depot   = inst.depot();          // 1
    const int n       = inst.dimension() - 1;  // nº de clientes
    const auto& dem   = inst.demands();

    std::vector<bool> served(inst.dimension() + 1, false);
    served[depot] = true;

    Solution sol;

    int remaining = n;
    while (remaining > 0) {
        Route r;
        int load   = 0;
        int curr   = depot;
        double len = 0.0;

        while (true) {
            // buscar cliente más cercano aún no servido que quepa
            int best = -1;
            double bestDist = std::numeric_limits<double>::max();

            for (int cid = 2; cid <= n + 1; ++cid) {
                if (served[cid]) continue;
                if (load + dem[cid - 1] > inst.capacity()) continue;

                double d = inst.dist(curr, cid);
                if (d < bestDist) {
                    bestDist = d;
                    best     = cid;
                }
            }

            if (best == -1) break;               // no cabe ninguno más

            // atender cliente 'best'
            served[best] = true;
            --remaining;

            r.customers.push_back(best);
            load += dem[best - 1];
            len  += bestDist;

            curr = best;
        }

        // cerrar ruta y volver al depósito
        len += inst.dist(curr, depot);

        r.load   = load;
        r.length = len;
        sol.addRoute(std::move(r));
    }
    return sol;
}

} // namespace cvrp
