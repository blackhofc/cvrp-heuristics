#pragma once
#include <string>
#include <vector>
#include <io/VRPLIBReader.h>

namespace cvrp {

class Instance {
public:
    explicit Instance(std::string path)
        : reader(std::move(path)) {}

    // Thin wrappers around VRPLIBReader -----------------------------
    const std::string&  name()       const { return reader.getName(); }
    int                 dimension()  const { return reader.getDimension(); }
    int                 capacity()   const { return reader.getCapacity(); }
    int                 vehiclesUB() const { return reader.getNumVehicles(); }
    int                 depot()      const { return reader.getDepotId(); }
    const std::vector<Node>& nodes() const { return reader.getNodes(); }
    const std::vector<int>&  demands()const{ return reader.getDemands(); }
    double dist(int i,int j) const   { return reader.getDistanceMatrix()[i][j]; }

private:
    VRPLIBReader reader;
};

} // namespace cvrp
