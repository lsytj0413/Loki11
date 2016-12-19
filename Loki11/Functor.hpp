// @file Functor.hpp
// @brief 仿函数
// @author
// @version
// @date
// @c++11 使用std::function

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
// @comment 因为Functor类会使用shared_ptr<FunctorImpl>存储不同的子类对象
template <typename... R>
class FunctorImpl;

template <typename R, typename... TArgs>
class FunctorImpl<R, Typelist<TArgs...>>
{
public:
    using ResultType = R;

    // @function operator()
    // @brief 调用函数, 子类必须实现此函数
    virtual R operator() (TArgs&&...) = 0;
    virtual ~FunctorImpl() {};
};


// @class FunctorHandler
// @brief 普通函数和函数对象实现类
// @comment FunctorImpl的子类
template <class ParentFunctor, typename Fun, typename... TArgs>
class FunctorHandler : public ParentFunctor::Impl
{
private:
    // @variale
    // @brief 包装的函数, 可以是函数指针, 仿函数对象以及 Functor
    Fun m_fn;

public:
    using Base = typename ParentFunctor::Impl;
    using ResultType = typename Base::ResultType;

    FunctorHandler(const Fun& fun)
            : m_fn(fun)
    {};

    // @function operator()
    // @brief 实现父类的纯虚函数
    ResultType operator()(TArgs&&... args) override {
        return m_fn(std::forward<TArgs>(args)...);
    };
};


// @class MemFunctorHandler
// @brief 成员函数实现类
// @comment FunctorImpl的子类
template <class ParentFunctor, typename PointorToObj, typename PointerToMemFn, typename... TArgs>
class MemFunctorHandler : public ParentFunctor::Impl
{
private:
    // @variale
    // @brief 对象指针
    PointorToObj m_obj;

    // @variale
    // @brief 对象函数指针
    PointerToMemFn m_mem_fn;

public:
    using Base = typename ParentFunctor::Impl;
    using ResultType = typename Base::ResultType;

    MemFunctorHandler(const PointorToObj& obj, PointerToMemFn mem_fn)
            : m_obj(obj)
            , m_mem_fn(mem_fn)
    {};

    // @function operator()
    // @brief 实现父类的纯虚函数
    ResultType operator()(TArgs&&... args) override {
        return (((*m_obj).*m_mem_fn)(std::forward<TArgs>(args)...));
    }
};


// @class Functor
// @brief 仿函数对象实现
template <typename R, typename... TArgs>
class Functor
{
public:
    using Impl = FunctorImpl<R, Typelist<TArgs...>>;
    using ResultType = R;
    using Parm1 = typename TypeAtNonStrict<Typelist<TArgs...>, 0>::Result;
    using ArgsList = Typelist<TArgs...>;

private:
    // @variale
    // @brief 保存的FunctorImpl 对象指针
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

    // @function operator()
    // @brief 调用函数
    R operator() (TArgs&&... args) {
        return (*m_impl)(std::forward<TArgs>(args)...);
    };

    // @function template.operator()
    // @brief 模板调用函数
    // @comment 成员函数模板, 支持变量的引用类型转换, 解决BinderFirst中的编译问题
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


// @class BinderFirst
// @brief 绑定第一个参数类
// @comment 模拟Functor
// OriginalFunctor 为FunctorImpl, 即BoundFunctorType::Impl
template <class OriginalFunctor, typename Head, typename... TArgs>
class BinderFirst : public Private::BinderFirstTraits<OriginalFunctor>::Impl
{
public:
    using Base = typename Private::BinderFirstTraits<OriginalFunctor>::Impl;
    using ResultType = typename OriginalFunctor::ResultType;
    using BoundType = typename OriginalFunctor::Parm1;

private:
    // @variale
    // @brief Functor, 即未绑定参数前的Functor对象
    OriginalFunctor m_fn;

    // @variale
    // @brief 被绑定的第一个值
    BoundType m_v;

public:
    BinderFirst(const OriginalFunctor& fn, BoundType b)
            : m_fn(fn)
            , m_v(b)
    {};

    // @function operator()
    // @brief 调用函数
    ResultType operator()(TArgs&&... args) {

        // @scheme 1
        // auto v{m_v}; // 不这样写不能通过编译, 因为Functor的operator已经实例化, 不再支持引用转换
        // return m_fn(std::move(v), std::forward<TArgs>(args)...);

        // @scheme 2
        // @brief 可能转而调用template.operator()
        // @comment 如果连续使用BinderFirst, 则可能出现编译错误
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
