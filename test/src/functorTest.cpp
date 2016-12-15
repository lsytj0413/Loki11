#include "functorTest.h"

#include <string>
#include <functional>
using std::function;
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


int test_multiargs_origin(int i, int j, int k) {
    return i + j + k;
}

string test_multiargs_origin_str(string i, const string& j) {
    return i + j;
}

TEST_F(FunctorTest, testOriginFunctionMultiArgs)
{
    auto f = Functor<int, int, int, int>(test_multiargs_origin);
    auto i = f(1, 10, 100);

    EXPECT_EQ(111, i);

    auto f2 = Functor<string, string, string>(test_multiargs_origin_str);
    auto i2 = f2("xxx", "yy");

    EXPECT_EQ("xxxyy", i2);
}


TEST_F(FunctorTest, testFunctorNonArgs)
{
    auto f = Functor<int>(std::function<int(void)>(test_noargs_origin));
    auto i = f();

    EXPECT_EQ(1, i);
}


TEST_F(FunctorTest, testFunctorOneArgs)
{
    auto f = Functor<int, int>(std::function<int(int)>(test_oneargs_origin));
    auto i = f(1);

    EXPECT_EQ(1, i);

    auto f2 = Functor<string, string>(std::function<string(string)>(test_oneargs_origin_str));
    auto i2 = f2("xxx");

    EXPECT_EQ("xxx", i2);
}
