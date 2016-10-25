#pragma once

#include <type_traits>

#include "NullType.hpp"


namespace Loki11
{

template <typename... TArgs>
struct Typelist {};


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

template <typename... T, typename... TArgs>
struct Append<Typelist<TArgs...>, Typelist<T...>>
{
    using Result = Typelist<TArgs..., T...>;
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


template <typename... T>
struct NoDuplicatesImp;

template <typename... UArgs>
struct NoDuplicatesImp<Typelist<>, Typelist<UArgs...>>
{
    using Result = Typelist<UArgs...>;
};

template <typename Head, typename... TArgs, typename... UArgs>
struct NoDuplicatesImp<Typelist<Head, TArgs...>, Typelist<UArgs...>>
        : public std::conditional<(-1 == IndexOf<Typelist<UArgs...>, Head>::value),
        NoDuplicatesImp<Typelist<TArgs...>, Typelist<UArgs..., Head>>,
        NoDuplicatesImp<Typelist<TArgs...>, Typelist<UArgs...>>
                                  >::type
{};

template <typename... TArgs>
struct NoDuplicates : public NoDuplicatesImp<Typelist<TArgs...>, Typelist<>>
{};


template <typename... T>
struct ReplaceImp;

template <typename T, typename U, typename... UArgs>
struct ReplaceImp<Typelist<>, T, U, Typelist<UArgs...>>
{
    using Result = Typelist<UArgs...>;
};

template <typename... TArgs, typename T, typename U, typename... UArgs>
struct ReplaceImp<Typelist<T, TArgs...>, T, U, Typelist<UArgs...>>
{
    using Result = Typelist<UArgs..., U, TArgs...>;
};

template <typename Head, typename... TArgs, typename T, typename U, typename... UArgs>
struct ReplaceImp<Typelist<Head, TArgs...>, T, U, Typelist<UArgs...>>
        : public ReplaceImp<Typelist<TArgs...>, T, U, Typelist<UArgs..., Head>>
{};

template <typename... T>
struct Replace;

template <typename... TArgs, typename T, typename U>
struct Replace<Typelist<TArgs...>, T, U>
        : public ReplaceImp<Typelist<TArgs...>, T, U, Typelist<>>
{};

template <typename... T>
struct ReplaceAllImp;

template <typename T, typename U, typename... UArgs>
struct ReplaceAllImp<Typelist<>, T, U, Typelist<UArgs...>>
{
    using Result = Typelist<UArgs...>;
};

template <typename... TArgs, typename T, typename U, typename... UArgs>
struct ReplaceAllImp<Typelist<T, TArgs...>, T, U, Typelist<UArgs...>>
        : public ReplaceAllImp<Typelist<TArgs...>, T, U, Typelist<UArgs..., U>>
{};

template <typename Head, typename... TArgs, typename T, typename U, typename... UArgs>
struct ReplaceAllImp<Typelist<Head, TArgs...>, T, U, Typelist<UArgs...>>
        : public ReplaceAllImp<Typelist<TArgs...>, T, U, Typelist<UArgs..., Head>>
{};

template <typename... T>
struct ReplaceAll;

template <typename... TArgs, typename T, typename U>
struct ReplaceAll<Typelist<TArgs...>, T, U>
        : public ReplaceAllImp<Typelist<TArgs...>, T, U, Typelist<>>
{};


template <typename... T>
struct Reverse;

template <>
struct Reverse<Typelist<>>
{
    using Result = Typelist<>;
};

template <typename Head, typename... TArgs>
struct Reverse<Typelist<Head, TArgs...>>
{
    using Result = typename Append<Typelist<TArgs...>, Head>::Result;
};


template <typename... T>
struct MostDerived;

template <typename T>
struct MostDerived<Typelist<>, T>
{
    using Result = T;
};

template <typename Head, typename... TArgs, typename T>
struct MostDerived<Typelist<Head, TArgs...>, T>
        : public std::conditional<std::is_base_of<T, Head>::value,
                                  MostDerived<Typelist<TArgs...>, Head>,
                                  MostDerived<Typelist<TArgs...>, T>
                                  >::type
{};


template <typename... T>
struct DerivedToFront;

template <>
struct DerivedToFront<Typelist<>>
{
    using Result = Typelist<>;
};

template <typename T>
struct DerivedToFront<Typelist<T>>
{
    using Result = Typelist<T>;
};

template <typename Head, typename... TArgs>
struct DerivedToFront<Typelist<Head, TArgs...>>
{
private:
    using TheMostDerived = typename MostDerived<Typelist<TArgs...>, Head>::Result;

public:
    using Result = typename Append<Typelist<TheMostDerived>,
                                   typename DerivedToFront<typename Replace<Typelist<TArgs...>,
                                                                            TheMostDerived, Head>::Result
                                                           >::Result>::Result;
};


}
