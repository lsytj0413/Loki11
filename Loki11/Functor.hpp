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
    virtual FunctorImpl* Clone() const = 0;
    virtual ~FunctorImpl() {};
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
    Functor& operator=(const Functor&);
    explicit Functor(std::unique_ptr<Impl> spImpl);

    R operator() (TArgs&&... args) {
        return (*m_impl)(std::forward<TArgs>(args)...);
    };
};




}
