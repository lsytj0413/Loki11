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


template <class ParentFunctor, typename Fun>
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

    ResultType operator()(){
    };
};


template <class ParentFunctor, typename PointorToObj, typename PointerToMemFn>
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

    ResultType operator()() {
        return ((*m_obj).*m_mem_fn());
    }
};


template <typename R, typename... TArgs>
class Functor
{
private:
    using Impl = FunctorImpl<R, TArgs...>;
    std::unique_ptr<Impl> m_impl;

public:
    Functor();
    Functor(const Functor&);
    Functor(Functor&&);
    Functor& operator=(const Functor&);
    Functor& operator=(Functor&&);
    explicit Functor(std::unique_ptr<Impl> spImpl);

    R operator() (TArgs&&... args) {
        return (*m_impl)(std::forward<TArgs>(args)...);
    };
};




}
