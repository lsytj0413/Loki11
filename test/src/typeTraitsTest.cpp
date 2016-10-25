#include "typeTraitsTest.h"

#include <Loki11/TypeTraits.hpp>
using namespace Loki11;


TEST_F(TypeTraitsTest, testPointer)
{
    auto v = TypeTraits<int>::isPointer;
    EXPECT_EQ(v, false);
    auto b = std::is_same<TypeTraits<int>::PointeeType, int*>::value;
    EXPECT_EQ(b, true);

    v = TypeTraits<int*>::isPointer;
    EXPECT_EQ(v, true);
    b = std::is_same<TypeTraits<int*>::PointeeType, int*>::value;
    EXPECT_EQ(b, true);
}


TEST_F(TypeTraitsTest, testReferenct)
{
    auto v = TypeTraits<int>::isReference;
    EXPECT_EQ(v, false);
    auto b = std::is_same<TypeTraits<int>::ReferredType, int&>::value;
    EXPECT_EQ(b, true);

    v = TypeTraits<int&>::isReference;
    EXPECT_EQ(v, true);
    b = std::is_same<TypeTraits<int&>::ReferredType, int&>::value;
    EXPECT_EQ(b, true);

    v = TypeTraits<int&&>::isReference;
    EXPECT_EQ(v, true);
    b = std::is_same<TypeTraits<int&&>::ReferredType, int&&>::value;
    EXPECT_EQ(b, true);
}


TEST_F(TypeTraitsTest, testMemberPointer)
{
    class C {
        void f(){};
    };
    auto v = TypeTraits<int*>::isMemberPointer;
    EXPECT_EQ(v, false);

    using F = void(C::*)();
    v = TypeTraits<F>::isMemberPointer;
    EXPECT_EQ(v, true);
}

TEST_F(TypeTraitsTest, testStdUInt)
{
    auto v = TypeTraits<unsigned char>::isStdUnsignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<unsigned short>::isStdUnsignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<unsigned int>::isStdUnsignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<unsigned long>::isStdUnsignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<unsigned long long>::isStdUnsignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<char>::isStdUnsignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<short>::isStdUnsignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<int>::isStdUnsignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<long>::isStdUnsignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<long long>::isStdUnsignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<bool>::isStdUnsignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<wchar_t>::isStdUnsignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<float>::isStdUnsignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<double>::isStdUnsignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<long double>::isStdUnsignedInt;
    EXPECT_EQ(v, false);
}

TEST_F(TypeTraitsTest, testStdSInt)
{
    auto v = TypeTraits<char>::isStdSignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<short>::isStdSignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<int>::isStdSignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<long>::isStdSignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<long long>::isStdSignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<bool>::isStdSignedInt;
    EXPECT_EQ(v, false);

    v = TypeTraits<wchar_t>::isStdSignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<float>::isStdSignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<double>::isStdSignedInt;
    EXPECT_EQ(v, true);

    v = TypeTraits<long double>::isStdSignedInt;
    EXPECT_EQ(v, true);
}

TEST_F(TypeTraitsTest, testStdIntegral)
{
    auto v = TypeTraits<unsigned char>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<unsigned short>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<unsigned int>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<unsigned long>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<unsigned long long>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<char>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<short>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<int>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<long>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<long long>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<bool>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<wchar_t>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<float>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<double>::isStdIntegral;
    EXPECT_EQ(v, true);

    v = TypeTraits<long double>::isStdIntegral;
    EXPECT_EQ(v, true);
}

TEST_F(TypeTraitsTest, testStdFloat)
{
    auto v = TypeTraits<int>::isStdFloat;
    EXPECT_EQ(v, false);

    v = TypeTraits<float>::isStdFloat;
    EXPECT_EQ(v, true);

    v = TypeTraits<double>::isStdFloat;
    EXPECT_EQ(v, true);

    v = TypeTraits<long double>::isStdFloat;
    EXPECT_EQ(v, true);

    v = TypeTraits<long long>::isStdFloat;
    EXPECT_EQ(v, false);

    v = TypeTraits<char>::isStdFloat;
    EXPECT_EQ(v, false);

    v = TypeTraits<unsigned short>::isStdFloat;
    EXPECT_EQ(v, false);
}

TEST_F(TypeTraitsTest, testParameterType)
{
    auto v = std::is_same<TypeTraits<int>::ParameterType, int>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<TypeTraits<int*>::ParameterType, int*>::value;
    EXPECT_EQ(v, true);

    class C{};

    v = std::is_same<TypeTraits<void(C::*)()>::ParameterType, void(C::*)()>::value;
    EXPECT_EQ(v, true);

    v = std::is_same<TypeTraits<C>::ParameterType, C&>::value;
    EXPECT_EQ(v, true);
}

TEST_F(TypeTraitsTest, testConst)
{
    auto v = TypeTraits<int>::isConst;
    EXPECT_EQ(v, false);
    auto b = std::is_same<TypeTraits<int>::NonConstType, int>::value;
    EXPECT_EQ(b, true);

    v = TypeTraits<int const>::isConst;
    EXPECT_EQ(v, true);
    b = std::is_same<TypeTraits<int const>::NonConstType, int>::value;
    EXPECT_EQ(b, true);
}

TEST_F(TypeTraitsTest, testVolatile)
{
    auto v = TypeTraits<int>::isVolatile;
    EXPECT_EQ(v, false);
    auto b = std::is_same<TypeTraits<int>::NonVolatileType, int>::value;
    EXPECT_EQ(b, true);

    v = TypeTraits<int volatile>::isVolatile;
    EXPECT_EQ(v, true);
    b = std::is_same<TypeTraits<int volatile>::NonVolatileType, int>::value;
    EXPECT_EQ(b, true);
}
