#pragma once
#include <vector>

namespace cvrp {

struct Route {
    std::vector<int> customers;   // depot NOT stored here
    int    load   {0};            // sum of customer demands
    double length {0.0};          // cached distance incl. depot legs

    int head() const { return customers.front(); }
    int tail() const { return customers.back();  }

    /// Append a customer after `prevId`.
    /// Arguments:
    ///   cid         – customer node id
    ///   demand      – its demand
    ///   distPrevCur – distance(prevId, cid)
    ///   distCurNext – distance(cid, nextId)   (nextId == depot if tail)
    ///   distPrevNextOld – distance(prevId, nextId) before insertion
    void insert(int cid,
                int demand,
                double distPrevCur,
                double distCurNext,
                double distPrevNextOld)
    {
        customers.push_back(cid);
        load   += demand;
        length += distPrevCur + distCurNext - distPrevNextOld;
    }
};

} // namespace cvrp
