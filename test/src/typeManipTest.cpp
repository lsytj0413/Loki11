#include "typeManipTest.h"

#include <Loki11/TypeManip.hpp>
using namespace Loki11;


TEST_F(TypeManipTest, testInt2Type)
{
    using T1 = Int2Type<0>;
    EXPECT_EQ(T1::value, 0);

    using T2 = Int2Type<3>;
    EXPECT_EQ(T2::value, 3);

    auto v = std::is_same<T1, T2>::value;
    EXPECT_EQ(false, v);
}
