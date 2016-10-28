#include "typeManipTest.h"

#include <Loki11/TypeManip.hpp>
using namespace Loki11;

#include <string>
using std::string;


TEST_F(TypeManipTest, testInt2Type)
{
    using T1 = Int2Type<0>;
    EXPECT_EQ(T1::value, 0);

    using T2 = Int2Type<3>;
    EXPECT_EQ(T2::value, 3);

    auto v = std::is_same<T1, T2>::value;
    EXPECT_EQ(false, v);
}


TEST_F(TypeManipTest, testType2Type)
{
    using T1 = Type2Type<int>;
    auto v = std::is_same<T1::OriginalType, int>::value;
    EXPECT_EQ(v, true);

    using T2 = Type2Type<double>;
    v = std::is_same<T2::OriginalType, double>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<T1, T2>::value;
    EXPECT_EQ(false, v);
}


TEST_F(TypeManipTest, testSelect)
{
    using T1 = Select<true, int, double>::Result;
    auto v = std::is_same<T1, int>::value;
    EXPECT_EQ(v, true);

    using T2 = Select<false, int, double>::Result;
    v = std::is_same<T2, double>::value;
    EXPECT_EQ(v, true);
}


TEST_F(TypeManipTest, testConversionExists)
{
    using T1 = Conversion<char, int>;
    auto v = T1::exists;
    EXPECT_EQ(v, true);

    using T2 = Conversion<string, int>;
    v = T2::exists;
    EXPECT_EQ(v, false);

    using T3 = Conversion<int, void>;
    v = T3::exists;
    EXPECT_EQ(false, v);

    using T4 = Conversion<void, int>;
    v = T4::exists;
    EXPECT_EQ(v, false);

    using T5 = Conversion<void, void>;
    v = T5::exists;
    EXPECT_EQ(v, true);
}


TEST_F(TypeManipTest, testConversionExists2Way)
{
    using T1 = Conversion<char, int>;
    auto v = T1::exists;
    EXPECT_EQ(v, true);
    v = T1::exists2Way;
    EXPECT_EQ(v, true);

    using T2 = Conversion<char*, void*>;
    v = T2::exists;
    EXPECT_EQ(v, true);
    v = T2::exists2Way;
    EXPECT_EQ(v, false);

    using T3 = Conversion<int, void>;
    v = T3::exists2Way;
    EXPECT_EQ(false, v);

    using T4 = Conversion<void, int>;
    v = T4::exists2Way;
    EXPECT_EQ(v, false);

    using T5 = Conversion<void, void>;
    v = T5::exists2Way;
    EXPECT_EQ(v, true);
}


TEST_F(TypeManipTest, testSuperClass)
{
    class B {};
    class D : public B {};
    class E {};

    auto v = SUPERSUBCLASS(B, D);
    EXPECT_EQ(v, true);

    v = SUPERSUBCLASS(B, E);
    EXPECT_EQ(v, false);

    v = SUPERSUBCLASS(D, E);
    EXPECT_EQ(v, false);
}
