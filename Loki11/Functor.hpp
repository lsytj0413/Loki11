// @file Functor.h
// @brief 仿函数
// @author
// @version
// @date

#pragma once

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

}
