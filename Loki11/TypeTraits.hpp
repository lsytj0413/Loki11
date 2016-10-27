// @file TypeTraits.hpp
// @brief 类型萃取
// @author
// @version
// @date

#pragma once

#include <type_traits>


namespace Loki11
{

// @brief以下三个用户可以偏特化
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


// @class TypeTraits
// @brief 类型萃取模板
// @c++11 使用<type_traits>中的模板
template <typename T>
class TypeTraits
{
public:
    // @variable
    // @brief 是否是指针类型
    static const bool isPointer = std::is_pointer<T>::value;
    // @type
    // @brief T对应的指针类型
    using PointeeType = typename std::conditional<isPointer,
                                                  T,
                                                  typename std::add_pointer<T>::type
                                                  >::type;

    // @variable
    // @brief 是否是引用类型
    static const bool isReference = std::is_reference<T>::value;
    // @type
    // @brief T对应的引用类型
    using ReferredType = typename std::conditional<isReference,
                                                   T,
                                                   typename std::add_lvalue_reference<T>::type
                                                   >::type;

    // @variable
    // @brief 是否是成员对象指针
    static const bool isMemberPointer = std::is_member_pointer<T>::value;

    // @variable
    // @brief 是否是标准unsigned类型
    static const bool isStdUnsignedInt = std::is_unsigned<T>::value;
    // @variable
    // @brief 是否是标准signed类型
    static const bool isStdSignedInt = std::is_signed<T>::value;
    // @variable
    // @brief 是否是标准integral类型
    static const bool isStdIntegral = isStdSignedInt ||
                                      isStdUnsignedInt ||
                                      std::is_same<T, char>::value ||
                                      std::is_same<T, wchar_t>::value ||
                                      std::is_same<T, bool>::value;
    // @variable
    // @brief 是否是标准浮点类型
    static const bool isStdFloat = std::is_same<T, float>::value ||
                                   std::is_same<T, double>::value ||
                                   std::is_same<T, long double>::value;
    // @variable
    // @brief 是否是标准算术类型
    static const bool isStdArith = isStdFloat || isStdIntegral;
    // @variable
    // @brief 是否是标准类型
    static const bool isStdFundamental = std::is_fundamental<T>::value;

    // @variable
    // @brief 是否是unsigned类型
    static const bool isUnsignedInt = isStdUnsignedInt || IsCustomUnsignedInt<T>::value;
    // @variable
    // @brief 是否是signed类型
    static const bool isSignedInt = isStdSignedInt || IsCustomSignedInt<T>::value;
    // @variable
    // @brief 是否是integral类型
    static const bool isIntegral = isStdIntegral ||
                                   isUnsignedInt ||
                                   isSignedInt;
    // @variable
    // @brief 是否是浮点类型
    static const bool isFloat = isStdFloat || IsCustomFloat<T>::value;
    // @variable
    // @brief 是否是算术类型
    static const bool isArith = isIntegral || isFloat;
    // @variable
    // @brief 是否是标准类型(包含可转换为)
    static const bool isFundamental = isStdFundamental ||
                                      isArith ||
                                      isFloat;

    // @type
    // @brief T的合适的参数类型
    using ParameterType = typename std::conditional<isStdArith || isPointer || isMemberPointer,
                                                    T,
                                                    typename std::add_lvalue_reference<T>::type
                                                    >::type;

    // @variable
    // @brief 是否const类型
    static const bool isConst = std::is_const<T>::value;
    // @type
    // @brief T对应的NonConst类型
    using NonConstType = typename std::conditional<isConst,
                                                   typename std::remove_const<T>::type,
                                                   T
                                                   >::type;

    // @variable
    // @brief 是否volatile类型
    static const bool isVolatile = std::is_volatile<T>::value;
    // @type
    // @brief T对应的NonVolatile类型
    using NonVolatileType = typename std::conditional<isVolatile,
                                                      typename std::remove_volatile<T>::type,
                                                      T
                                                      >::type;
    // @type
    // @brief T对应的NonConst, NonVolatile类型
    using UnqualifiedType = typename std::remove_cv<T>::type;
};

}
