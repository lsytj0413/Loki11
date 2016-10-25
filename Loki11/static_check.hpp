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

// C++11: 使用 static_asser t
#define STATIC_CHECK(expr, msg)                     \
    { \
        class ERROR_##msg {}; \
        decltype(Loki11::CompileTimeChecker<(expr)>(ERROR_##msg())) v; (void)v; \
    }
