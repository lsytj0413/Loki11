#pragma once

#include <type_traits>

#include "NullType.hpp"


namespace Loki11
{

template <typename... TArgs>
struct Typelist
{};


template <class TL>
struct Length;

template <typename... TArgs>
struct Length<Typelist<TArgs...>>
{
    static const int value = sizeof...(TArgs);
};


template <typename T, unsigned int index>
struct TypeAt;

template <typename Head, typename... TArgs>
struct TypeAt<Typelist<Head, TArgs...>, 0>
{
    using Result = Head;
};

template <unsigned int i, typename Head, typename... TArgs>
struct TypeAt<Typelist<Head, TArgs...>, i>
{
    using Result = typename TypeAt<Typelist<TArgs...>, i-1>::Result;
};

template <typename T, unsigned int index, typename DefaultType = NullType>
struct TypeAtNonStrict
{
    using Result = DefaultType;
};

template <typename DefaultType, typename Head, typename... TArgs>
struct TypeAtNonStrict<Typelist<Head, TArgs...>, 0, DefaultType>
{
    using Result = Head;
};

template <typename DefaultType, unsigned int i, typename Head, typename... TArgs>
struct TypeAtNonStrict<Typelist<Head, TArgs...>, i, DefaultType>
{
    using Result = typename TypeAtNonStrict<Typelist<TArgs...>, i-1, DefaultType>::Result;
};


template <typename TL, typename T>
struct IndexOf;

template<typename T>
struct IndexOf<Typelist<>, T> : public std::integral_constant<int, -1>
{};

template <typename T, typename... TArgs>
struct IndexOf<Typelist<T, TArgs...>, T> : public std::integral_constant<int, 0>
{};

template <typename T, typename Head, typename... TArgs>
struct IndexOf<Typelist<Head, TArgs...>, T>
        : public std::conditional<(-1 == IndexOf<Typelist<TArgs...>, T>::value),
                std::integral_constant<int, -1>,
                std::integral_constant<int, 1 + IndexOf<Typelist<TArgs...>, T>::value>
          >::type
{};

template <typename... T>
struct Append;

template <typename T, typename... TArgs>
struct Append<Typelist<TArgs...>, T>
{
    using Result = Typelist<TArgs..., T>;
};


template <typename... T>
struct EraseImp;

template <typename T, typename... TArgs, typename... UArgs>
struct EraseImp<Typelist<T, TArgs...>, T, Typelist<UArgs...>>
{
    using Result = Typelist<UArgs..., TArgs...>;
};

template <typename T, typename Head, typename... TArgs, typename... UArgs>
struct EraseImp<Typelist<Head, TArgs...>, T, Typelist<UArgs...>>
{
    using Result = typename EraseImp<Typelist<TArgs...>, T, Typelist<UArgs..., Head>>::type;
};

template <typename... T>
struct Erase;

template <typename T, typename... TArgs>
struct Erase<Typelist<TArgs...>, T>
        : public EraseImp<Typelist<TArgs...>, T, Typelist<>>
{};


template <typename... T>
struct EraseAllImp;

template <typename T, typename... UArgs>
struct EraseAllImp<Typelist<>, T, Typelist<UArgs...>>
{
    using Result = Typelist<UArgs...>;
};

template <typename T, typename... TArgs, typename... UArgs>
struct EraseAllImp<Typelist<T, TArgs...>, T, Typelist<UArgs...>>
        : public EraseAllImp<Typelist<TArgs...>, T, Typelist<UArgs...>>
{};

template <typename T, typename Head, typename... TArgs, typename... UArgs>
struct EraseAllImp<Typelist<Head, TArgs...>, T, Typelist<UArgs...>>
        : public EraseAllImp<Typelist<TArgs...>, T, Typelist<UArgs..., Head>>
{};

template <typename... T>
struct EraseAll;

template <typename T, typename... TArgs>
struct EraseAll<Typelist<TArgs...>, T>
        : public EraseAllImp<Typelist<TArgs...>, T, Typelist<>>
{};



}
