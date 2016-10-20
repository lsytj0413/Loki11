#pragma once

#include <type_traits>

namespace Loki11
{

template <int v>
struct Int2Type : public std::integral_constant<int, v>
{};

template <typename T>
struct Type2Type
{
    using OriginalType = T;
};


// Select, C++11之后可以使用std::conditional
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

template <class T, class U>
class Conversion : public std::integral_constant<bool,
                                                 std::is_same<decltype(ConversionImp<U>::Test(std::declval<T>())),
                                                              std::true_type>::value
                                                 >
{
public:
    static const bool exists = Conversion<T, U>::value;
    static const bool exists2Way = exists && Conversion<U, T>::value;
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


// C++11: 可以使用std::is_base_of
// 测试T是否是U的父类
#define SUPERSUBCLASS(T, U) \
    (Conversion<const U*, const T*>::exists && \
     !std::is_same<const T*, const void*>::value)

#define SUPERSUBCLASS_STRICT(T, U) \
    (SUPERSUBCLASS(T, U) && \
     !std::is_same<const T, const U>::value)


}
