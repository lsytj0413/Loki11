// @file Functor.hpp
// @brief 仿函数
// @author
// @version
// @date

#pragma once

#include <memory>
#include <utility>
#include <cassert>
using std::shared_ptr;

#include "Typelist.hpp"


namespace Loki11
{

// @class FunctorImpl
// @brief Functor实现基类
template <typename... R>
class FunctorImpl;

template <typename R, typename... TArgs>
class FunctorImpl<R, Typelist<TArgs...>>
{
public:
    using ResultType = R;

    virtual R operator() (TArgs&&...) = 0;
    virtual ~FunctorImpl() {};
};


// @class FunctorHandler
// @brief 普通函数和函数对象实现类
template <class ParentFunctor, typename Fun, typename... TArgs>
class FunctorHandler : public ParentFunctor::Impl
{
private:
    Fun m_fn;

public:
    using Base = typename ParentFunctor::Impl;
    using ResultType = typename Base::ResultType;

    FunctorHandler(const Fun& fun)
            : m_fn(fun)
    {};

    ResultType operator()(TArgs&&... args){
        return m_fn(std::forward<TArgs>(args)...);
    };
};


// @class MemFunctorHandler
// @brief 成员函数实现类
template <class ParentFunctor, typename PointorToObj, typename PointerToMemFn, typename... TArgs>
class MemFunctorHandler : public ParentFunctor::Impl
{
private:
    PointorToObj m_obj;
    PointerToMemFn m_mem_fn;

public:
    using Base = typename ParentFunctor::Impl;
    using ResultType = typename Base::ResultType;

    MemFunctorHandler(const PointorToObj& obj, PointerToMemFn mem_fn)
            : m_obj(obj)
            , m_mem_fn(mem_fn)
    {};

    ResultType operator()(TArgs&&... args) {
        return (((*m_obj).*m_mem_fn)(std::forward<TArgs>(args)...));
    }
};


// @class Functor
// @brief 仿函数实现
template <typename R, typename... TArgs>
class Functor
{
public:
    using Impl = FunctorImpl<R, Typelist<TArgs...>>;
    using ResultType = R;
    using Parm1 = typename TypeAtNonStrict<Typelist<TArgs...>, 0>::Result;
    using ArgsList = Typelist<TArgs...>;

private:
    std::shared_ptr<Impl> m_impl;

public:
    Functor()
            : m_impl(nullptr)
    {};
    Functor(const Functor& rhs)
            : m_impl(rhs.m_impl)
    {} ;
    Functor(Functor&& rhs)
            : m_impl(std::move(rhs.m_impl))
    {};
    Functor& operator=(const Functor& rhs) {
        m_impl = rhs.m_impl;
    };
    Functor& operator=(Functor&& rhs){
        m_impl = std::move(rhs.m_impl);
    };
    explicit Functor(const std::shared_ptr<Impl>& spImpl)
            : m_impl(spImpl)
    {};

    template <typename Fun>
    Functor(Fun fn)
            : m_impl(new FunctorHandler<Functor, Fun, TArgs...>(fn))
    {};

    template <typename PtrObj, typename MemFn>
    Functor(const PtrObj& p, MemFn memFn)
            :m_impl(new MemFunctorHandler<Functor, PtrObj, MemFn, TArgs...>(p, memFn))
    {};

    R operator() (TArgs&&... args) {
        return (*m_impl)(std::forward<TArgs>(args)...);
    };

    // 支持类型转换, 已经提供另一个实例化可能性
    template <typename... UArgs>
    R operator()(UArgs&&... args) {
        return (*m_impl)(std::forward<TArgs>(args)...);
    }
};


namespace Private
{

template <typename... Fctor>
struct BinderFirstTraits;

template <typename R, typename Head, typename... TL>
struct BinderFirstTraits<Functor<R, Head, TL...>>
{
    using ArgsList = Typelist<TL...>;
    using ParmList = Typelist<TL...>;
    using BoundFunctorType = Functor<R, TL...>;
    using Impl = typename BoundFunctorType::Impl;
};

}

// OriginalFunctor 为FunctorImpl, 即BoundFunctorType::Impl
template <class OriginalFunctor, typename Head, typename... TArgs>
class BinderFirst : public Private::BinderFirstTraits<OriginalFunctor>::Impl
{
public:
    using Base = typename Private::BinderFirstTraits<OriginalFunctor>::Impl;
    using ResultType = typename OriginalFunctor::ResultType;
    using BoundType = typename OriginalFunctor::Parm1;

private:
    OriginalFunctor m_fn;    // Functor
    BoundType m_v;

public:
    BinderFirst(const OriginalFunctor& fn, BoundType b)
            : m_fn(fn)
            , m_v(b)
    {};

    ResultType operator()(TArgs&&... args) {
        // auto v{m_v};          // 不这样写不能通过编译, 因为Functor的operator已经实例化...
        // return m_fn(std::move(v), std::forward<TArgs>(args)...);
        return m_fn(m_v, std::forward<TArgs>(args)...);
    }
};


namespace Private
{

template <class Fctor, typename... TArgs>
auto BindFirst(const Fctor& fn, typename Fctor::Parm1 bound, Typelist<TArgs...>)
{
    // BoundFunctorType: 为使用fn和剩余的参数构造的Functor
    using Outgoing = typename Private::BinderFirstTraits<Fctor>::BoundFunctorType;
    using BinderType = BinderFirst<Fctor, typename Fctor::Parm1, TArgs...>;

    return Outgoing(std::shared_ptr<typename Outgoing::Impl>(
        new BinderType(fn, bound)));
}

}


template <class Fctor>
auto BindFirst(const Fctor& fn, typename Fctor::Parm1 bound)
{
    using RestArgsList = typename Erase<typename Fctor::ArgsList,
                                        typename Fctor::Parm1>::Result;

    return Private::BindFirst(fn, bound, RestArgsList{});
}


}
