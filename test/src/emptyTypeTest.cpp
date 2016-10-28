#include "emptyTypeTest.h"

#include <Loki11/EmptyType.hpp>


TEST_F(EmptyTypeTest, test)
{
    Loki11::EmptyType t;
    EXPECT_TRUE(sizeof(t) > 0);
}
