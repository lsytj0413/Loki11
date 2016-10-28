#include "nullTypeTest.h"

#include <Loki11/NullType.hpp>


TEST_F(NullTypeTest, test)
{
    Loki11::NullType t;
    EXPECT_TRUE(sizeof(t) > 0);
}
