#pragma once


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

template <typename Head, typename Tail>
struct TypeAt<Typelist<Head, Tail>, 0>
{
    using Result = Head;
};

template <unsigned int i, typename Head, typename... TArgs>
struct TypeAt<Typelist<Head, TArgs...>, i>
{
    using Result = typename TypeAt<Typelist<TArgs...>, i-1>::Result;
};



}
