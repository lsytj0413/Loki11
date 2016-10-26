// @file static_check.h
// @brief 编译期assert
// @author
// @version
// @date

#pragma once


namespace Loki11
{

template <bool>
struct CompileTimeChecker
{
    CompileTimeChecker(...){};
};

template <>
struct CompileTimeChecker<false> {};

}

// @macro: STATIC_CHECK
// @brief: 编译期assert实现宏
// @C++11: 使用static_assert
#define STATIC_CHECK(expr, msg)                     \
    { \
        class ERROR_##msg {}; \
        decltype(Loki11::CompileTimeChecker<(expr)>(ERROR_##msg())) v; (void)v; \
    }
