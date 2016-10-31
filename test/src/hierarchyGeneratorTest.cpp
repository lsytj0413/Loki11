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

    v = std::is_same<GenScatterHierarchy<int, ScatUnit>, typename T1::LeftBase>::value;
    EXPECT_EQ(v, true);

    using T2 = GenScatterHierarchy<int, ScatUnit>;
    EXPECT_TRUE(sizeof(T2) == sizeof(int));

    auto v2 = T2();
    v = std::is_same<int, decltype(v2.i)>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<ScatUnit<int>, typename T2::LeftBase>::value;
    EXPECT_EQ(v, true);
}


TEST_F(HierarchyGenTest, testScatterMulti)
{
    using T1 = GenScatterHierarchy<Typelist<int, char>, ScatUnit>;
    using T2 = GenScatterHierarchy<int, ScatUnit>;
    using T3 = GenScatterHierarchy<Typelist<char>, ScatUnit>;

    auto v = std::is_same<T2, T1::LeftBase>::value;
    EXPECT_EQ(v, true);
    v = std::is_same<T3, T1::RightBase>::value;
    EXPECT_EQ(v, true);

    using T4 = GenScatterHierarchy<Typelist<int, char, double>, ScatUnit>;
    using T5 = GenScatterHierarchy<int, ScatUnit>;
    using T6 = GenScatterHierarchy<Typelist<char, double>, ScatUnit>;

    v = std::is_same<T5, T4::LeftBase>::value;
    EXPECT_EQ(v, true);
    v = std::is_same<T6, T4::RightBase>::value;
    EXPECT_EQ(v, true);
}


TEST_F(HierarchyGenTest, testFieldT)
{
    using T1 = GenScatterHierarchy<Typelist<int, char, double>, ScatUnit>;
    auto v = T1();

    auto v1 = Field<int>(v);
    auto b = std::is_same<int, decltype(v1.i)>::value;
    EXPECT_EQ(b, true);

    auto v2 = Field<char>(T1());
    b = std::is_same<char, decltype(v2.i)>::value;
    EXPECT_EQ(b, true);

    auto v3 = Field<double>(v);
    b = std::is_same<double, decltype(v3.i)>::value;
    EXPECT_EQ(b, true);

    // compile error:  ambiguous base
    // using T2 = GenScatterHierarchy<Typelist<int, char, int>, ScatUnit>;
    // auto v4 = Field<int>(T2());
    // b = std::is_same<double, decltype(v4.i)>::value;
    // EXPECT_EQ(b, true);
}


TEST_F(HierarchyGenTest, testFieldI)
{}


TEST_F(HierarchyGenTest, testTuple)
{}


TEST_F(HierarchyGenTest, testLinear)
{}
