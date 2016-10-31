#include "hierarchyGeneratorTest.h"

#include <Loki11/HierarchyGenerator.hpp>
using namespace Loki11;

template <typename T>
struct ScatUnit
{
public:
    T i;
};


TEST_F(HierarchyGenTest, testScatterZero)
{
    class Em {};

    using T1 = GenScatterHierarchy<Typelist<>, ScatUnit>;
    EXPECT_TRUE(sizeof(Em) == sizeof(T1));
}


TEST_F(HierarchyGenTest, testScatterOne)
{
    using T1 = GenScatterHierarchy<Typelist<int>, ScatUnit>;
    EXPECT_TRUE(sizeof(T1) == sizeof(int));

    auto v1 = T1();
    auto v = std::is_same<int, decltype(v1.i)>::value;
    EXPECT_EQ(v, true);

    using T2 = GenScatterHierarchy<int, ScatUnit>;
    EXPECT_TRUE(sizeof(T2) == sizeof(int));

    auto v2 = T2();
    v = std::is_same<int, decltype(v2.i)>::value;
    EXPECT_EQ(v, true);
}


TEST_F(HierarchyGenTest, testScatterMulti)
{}


TEST_F(HierarchyGenTest, testFieldT)
{}


TEST_F(HierarchyGenTest, testFieldI)
{}


TEST_F(HierarchyGenTest, testTuple)
{}


TEST_F(HierarchyGenTest, testLinear)
{}
