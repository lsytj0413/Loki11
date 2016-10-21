#pragma once


namespace Loki11
{

template <bool>
struct CompileTimeChecker
{
    CompileTimeChecker(...);
};

template <>
struct CompileTimeChecker<false> {};

// C++11: 使用 static_assert
#define STATIC_CHECK(expr, msg)                     \
    { \
        class ERROR_##MSG {}; \
        (void)sizeof(CompileTimeChecker<(expr)>(ERROR_##msg())); \
    }

}
