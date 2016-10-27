// @file TypeManip.hpp
// @brief 一些类型判断相关类
// @author
// @version
// @date

#pragma once

#include <type_traits>


namespace Loki11
{

// @struct Int2Type
// @brief 将整数转换为类型
// @c++11 使用std::integral_constant
template <int v>
struct Int2Type : public std::integral_constant<int, v>
{};


// @struct Type2Type
// @brief 将类型转换为类型
template <typename T>
struct Type2Type
{
    using OriginalType = T;
};


// @struct Select
// @brief 编译期if, 类型选择
// @c++11 使用std::conditional
template <bool flag, typename T, typename U>
struct Select
{
    using Result = T;
};

template <typename T, typename U>
struct Select<false, T, U>
{
    using Result = U;
};


template <class U>
struct ConversionImp
{
    static std::true_type Test(U);
    static std::false_type Test(...);
};

// @struct Conversion
// @brief 判断类型是否可隐式转换
// @c++11 使用std::is_convertible
template <class T, class U>
class Conversion : public std::integral_constant<bool,
                                                 std::is_same<decltype(ConversionImp<U>::Test(std::declval<T>())),
                                                              std::true_type>::value
                                                 >
{
public:
    // @variable
    // @brief T是否能转换为U
    static const bool exists = Conversion<T, U>::value;

    // @variable
    // @brief 是否可以互相转换
    static const bool exists2Way = exists && Conversion<U, T>::value;

    // @variable
    // @brief 是否为同一类型
    static const bool sameType = std::is_same<T, U>::value;
};

template <bool v>
class ConversionBase
{
public:
    static const bool exists = v;
    static const bool exists2Way = v;
    static const bool sameType = v;
};

template <class T>
class Conversion<T, void> : public ConversionBase<false>
{};

template <class T>
class Conversion<void, T> : public ConversionBase<false>
{};

template <>
class Conversion<void, void> : public ConversionBase<true>
{};


// @macro SUPERSUBCLASS
// @brief 测试T是否是U的父类
// @c++11 使用std::is_base_of
#define SUPERSUBCLASS(T, U) \
    (Conversion<const U*, const T*>::exists && \
     !std::is_same<const T*, const void*>::value)

// @macro SUPERSUBCLASS_STRICT
// @brief 测试T是否是U的父类, 并排除T==U
#define SUPERSUBCLASS_STRICT(T, U) \
    (SUPERSUBCLASS(T, U) && \
     !std::is_same<const T, const U>::value)


}
