#pragma once

#include <type_traits>


namespace Loki11
{

// 以下三个用户可以偏特化
template <typename T>
struct IsCustomUnsignedInt
{
    static const bool value = 0;
};

template <typename T>
struct IsCustomSignedInt
{
    static const bool value = 0;
};

template <typename T>
struct IsCustomFloat
{
    static const bool value = 0;
};


template <typename T>
class TypeTraits
{
public:
    static const bool isPointer = std::is_pointer<T>::value;
    using PointeeType = typename std::conditional<isPointer,
                                                  T,
                                                  typename std::add_pointer<T>::type
                                                  >::type;

    static const bool isReference = std::is_reference<T>::value;
    using ReferredType = typename std::conditional<isReference,
                                                   T,
                                                   typename std::add_lvalue_reference<T>::type
                                                   >::type;

    static const bool isMemberPointer = std::is_member_pointer<T>::value;

    static const bool isStdUnsignedInt = std::is_unsigned<T>::value;
    static const bool isStdSignedInt = std::is_signed<T>::value;
    static const bool isStdIntegral = isStdSignedInt ||
                                      isStdUnsignedInt ||
                                      std::is_same<T, char>::value ||
                                      std::is_same<T, wchar_t>::value ||
                                      std::is_same<T, bool>::value;
    static const bool isStdFloat = std::is_same<T, float>::value ||
                                   std::is_same<T, double>::value ||
                                   std::is_same<T, long double>::value;
    static const bool isStdArith = isStdFloat || isStdIntegral;
    static const bool isStdFundamental = std::is_fundamental<T>::value;

    static const bool isUnsignedInt = isStdUnsignedInt || IsCustomUnsignedInt<T>::value;
    static const bool isSignedInt = isStdSignedInt || IsCustomSignedInt<T>::value;
    static const bool isIntegral = isStdIntegral ||
                                   isUnsignedInt ||
                                   isSignedInt;
    static const bool isFloat = isStdFloat || IsCustomFloat<T>::value;
    static const bool isArith = isIntegral || isFloat;
    static const bool isFundamental = isStdFundamental ||
                                      isArith ||
                                      isFloat;

    using ParameterType = typename std::conditional<isStdArith || isPointer || isMemberPointer,
                                                    T,
                                                    typename std::add_lvalue_reference<T>::type
                                                    >::type;

    static const bool isConst = std::is_const<T>::value;
    using NonConstType = typename std::conditional<isConst,
                                                   typename std::remove_const<T>::type,
                                                   T
                                                   >::type;

    static const bool isVolatile = std::is_volatile<T>::value;
    using NonVolatileType = typename std::conditional<isVolatile,
                                                      typename std::remove_volatile<T>::type,
                                                      T
                                                      >::type;

    using UnqualifiedType = typename std::remove_cv<T>::type;
};

}
