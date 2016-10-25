#include "typeInfoTest.h"

#include <Loki11/LokiTypeInfo.hpp>
using namespace Loki11;


TEST_F(TypeInfoTest, TestNullCons)
{
    TypeInfo t;
    t.name();
    t.Get();
}

TEST_F(TypeInfoTest, TestCopyCons)
{
    TypeInfo t = TypeInfo(typeid(int));
    TypeInfo t1 = TypeInfo(t);

    t.name();
    t.Get();
    t1.name();
    t1.Get();
}

TEST_F(TypeInfoTest, TestEqualCons)
{
    TypeInfo t = TypeInfo(typeid(int));
    TypeInfo t1;

    t1 = t;
}

TEST_F(TypeInfoTest, TestBefore)
{
    TypeInfo t = TypeInfo(typeid(int));
    TypeInfo t1 = TypeInfo(typeid(char));

    t.before(t1);
    t1.before(t);
}

TEST_F(TypeInfoTest, TestName)
{
    TypeInfo t = TypeInfo(typeid(int));
    t.name();
}

TEST_F(TypeInfoTest, TestGet)
{
    TypeInfo t = TypeInfo(typeid(int));
    t.Get();
}

TEST_F(TypeInfoTest, TestCompair)
{
    TypeInfo t = TypeInfo(typeid(int));
    TypeInfo t1 = TypeInfo(typeid(char));

    EXPECT_EQ(t == t1, false);
    EXPECT_EQ(t != t1, true);

    auto v = (t < t1);
    auto v1 = (t <= t1);
    EXPECT_EQ(v, v1);

    v = (t > t1);
    v1 = (t >= t1);
    EXPECT_EQ(v, v1);

    v = (t < t1);
    v1 = (t > t1);
    EXPECT_EQ(!v, v1);
}

