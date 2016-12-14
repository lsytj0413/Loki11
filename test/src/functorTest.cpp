#include "functorTest.h"

#include <Loki11/Functor.hpp>
using namespace Loki11;


int test_noargs_origin() {
    return 1;
};

TEST_F(FunctorTest, testOriginFunctionNonArgs)
{
    auto f = Functor<int>(test_noargs_origin);
    auto i = f();

    EXPECT_EQ(1, i);
}
