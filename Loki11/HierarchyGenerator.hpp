#pragma once

#include "Typelist.hpp"
#include "TypeManip.hpp"

namespace Loki11
{

template <typename T, template <class> class Unit>
class GenScatterHierarchy;

template <typename T, template <class> class Unit>
class GenScatterHierarchy : public Unit<T>
{
public:
    using LeftBase = Unit<T>;
};

template <template <class> class Unit>
class GenScatterHierarchy<Typelist<>, Unit>
{};

template <typename Head, typename... TArgs, template <class> class Unit>
class GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>
        : public GenScatterHierarchy<Head, Unit>
        , public GenScatterHierarchy<Typelist<TArgs...>, Unit>
{
public:
    using LeftBase = GenScatterHierarchy<Head, Unit>;
    using RightBase = GenScatterHierarchy<Typelist<TArgs...>, Unit>;
};

template <class T, class... TArgs, template <class> class Unit>
Unit<T>& Field(GenScatterHierarchy<Typelist<TArgs...>, Unit>& obj)
{
    return obj;
};

template <template <class> class Unit, typename Head, typename... TArgs>
Unit<Head>& FieldHelper(GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>& obj,
                        Int2Type<0>)
{
    GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>& leftBase = obj;
    return leftBase;
};

template <int i, template <class> class Unit, typename Head, typename... TArgs>
Unit<Head>& FieldHelper(GenScatterHierarchy<Typelist<Head, TArgs...>, Unit>& obj,
                        Int2Type<i>)
{
    GenScatterHierarchy<Typelist<TArgs...>, Unit>& rightBase = obj;
    return FieldHelper(rightBase, Int2Type<i-1>());
};

template <int i, class... TArgs, template <class> class Unit>
Unit<typename TypeAt<Typelist<TArgs...>, i>::Result>&
Field(GenScatterHierarchy<Typelist<TArgs...>, Unit>& obj)
{
    return FieldHelper(obj, Int2Type<i>());
};

template <class T>
struct TupleUnit
{
    T value_;
    operator T&() {
        return value_;
    };
    operator const T&() const {
        return value_;
    };
};


template <typename TL>
struct Tuple : public GenScatterHierarchy<TL, TupleUnit>
{};

template <typename... TArgs>
struct Tuple<Typelist<TArgs...>> : public GenScatterHierarchy<Typelist<TArgs...>, TupleUnit>
{};

template <typename T, template <class, class> class Unit>
class GenLinearHierarchy;

template <template <class, class> class Unit>
class GenLinearHierarchy<Typelist<>, Unit>
{};

template <typename Head, typename... TArgs, template <class, class> class Unit>
class GenLinearHierarchy<Typelist<Head, TArgs...>, Unit>
        : public Unit<Head, GenLinearHierarchy<Typelist<TArgs...>, Unit>>
{};


}
