#include "functorTest.h"

#include <string>
#include <functional>
#include <cassert>
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


TEST_F(FunctorTest, testFunctorMultiArgs)
{
    auto f = Functor<int, int, int, int>(std::function<int(int, int, int)>(test_multiargs_origin));
    auto i = f(1, 10, 100);

    EXPECT_EQ(111, i);

    auto f2 = Functor<string, string, string>(std::function<string(string, string)>(test_multiargs_origin_str));
    auto i2 = f2("xxx", "yy");

    EXPECT_EQ("xxxyy", i2);
}


class TestFunctor
{
public:
    int test_noargs() {
        return 10;
    };

    int test_oneargs(int i) {
        return i;
    };

    string test_onargs_str(const string& s) {
        return s;
    };

    int test_multiargs(int i, int j, int k) {
        return i + j + k;
    };

    string test_multiargs_str(const string& i, const string& j) {
        return i + j;
    };
};


TEST_F(FunctorTest, testMemFunctorNonArgs)
{
    TestFunctor *obj = new TestFunctor;

    auto f = Functor<int>(obj, &TestFunctor::test_noargs);
    auto i = f();

    EXPECT_EQ(10, i);
}


TEST_F(FunctorTest, testMemFunctorOneArgs)
{
    TestFunctor *obj = new TestFunctor;

    auto f = Functor<int, int>(obj, &TestFunctor::test_oneargs);
    auto i = f(1);

    EXPECT_EQ(1, i);

    auto f2 = Functor<string, string>(obj, &TestFunctor::test_onargs_str);
    auto i2 = f2("xxx");

    EXPECT_EQ("xxx", i2);
}


TEST_F(FunctorTest, testMemFunctorMultiArgs)
{
    TestFunctor *obj = new TestFunctor;

    auto f = Functor<int, int, int, int>(obj, &TestFunctor::test_multiargs);
    auto i = f(1, 10, 100);

    EXPECT_EQ(111, i);

    auto f2 = Functor<string, string, string>(obj, &TestFunctor::test_multiargs_str);
    auto i2 = f2("xxx", "yy");

    EXPECT_EQ("xxxyy", i2);
}


TEST_F(FunctorTest, testBindFirstOneArgs)
{
    int v = 10;
    auto f = BindFirst(Functor<int, int>(test_oneargs_origin), v);
    // auto i = f();

    // EXPECT_EQ(i, 10);
}
