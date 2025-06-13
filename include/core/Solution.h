#pragma once
#include <vector>
#include <core/Route.h>

namespace cvrp {

class Solution {
public:
    std::vector<Route>&       routes()       { return _routes; }
    const std::vector<Route>& routes() const { return _routes; }

    double totalCost() const { return _total; }
    void   setTotalCost(double v) { _total = v; }

    void addRoute(Route r) {
        _total += r.length;
        _routes.push_back(std::move(r));
    }

private:
    std::vector<Route> _routes;
    double             _total{0.0};
};

} // namespace cvrp
