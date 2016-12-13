// @file Functor.h
// @brief 仿函数
// @author
// @version
// @date

#pragma once

#include <memory>
#include <utility>
using std::unique_ptr;

#include "Typelist.hpp"


namespace Loki11
{

template <typename... R>
class FunctorImpl;


template <typename R, typename... TArgs>
class FunctorImpl<R, Typelist<TArgs...>>
{
public:
    virtual R operator() (TArgs&&...) = 0;
    virtual ~FunctorImpl() {};
};


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
        return ((*m_obj).*m_mem_fn(std::forward<TArgs>(args)...));
    }
};


template <typename R, typename... TArgs>
class Functor
{
public:
    using Impl = FunctorImpl<R, TArgs...>;
    using ResultType = R;

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
    explicit Functor(std::shared_ptr<Impl>& spImpl)
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
};




}
