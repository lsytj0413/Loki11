#include "functorTest.h"

#include <string>
using std::string;

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


int test_oneargs_origin(int i) {
    return i;
}

string test_oneargs_origin_str(string i) {
    return i;
}

TEST_F(FunctorTest, testOriginFunctionOneArgs)
{
    auto f = Functor<int, int>(test_oneargs_origin);
    auto i = f(1);

    EXPECT_EQ(1, i);

    auto f2 = Functor<string, string>(test_oneargs_origin_str);
    auto i2 = f2("xxx");

    EXPECT_EQ("xxx", i2);
}
