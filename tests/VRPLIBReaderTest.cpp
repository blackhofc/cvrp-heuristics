#include <gtest/gtest.h>
#include <io/VRPLIBReader.h>

// Small built-in instance so the test is self-contained
static const char* mini =
R"(NAME : Tiny
COMMENT : 3-customer sample
TYPE : CVRP
DIMENSION : 4
EDGE_WEIGHT_TYPE : EUC_2D
CAPACITY : 99
NODE_COORD_SECTION
1 0 0
2 0 3
3 4 0
4 4 3
DEMAND_SECTION
1 0
2 10
3 20
4 30
DEPOT_SECTION
1
EOF)";

TEST(VRPLIBReader, DistanceSymmetry)
{
    // feed the string via an std::stringstream
    std::stringstream ss(mini);
    VRPLIBReader reader(ss);

    auto& D = reader.getDistanceMatrix();
    for (size_t i = 0; i < D.size(); ++i)
        for (size_t j = 0; j < D.size(); ++j)
            EXPECT_DOUBLE_EQ(D[i][j], D[j][i]);
}

TEST(VRPLIBReader, DemandSum)
{
    std::stringstream ss(mini);
    VRPLIBReader reader(ss);

    int sum = 0;
    for (int d : reader.getDemands()) sum += d;
    EXPECT_EQ(sum, 60);
}
