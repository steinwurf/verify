// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "../detail/fail.hpp"
#include "../verify_variadic_overload_macro.hpp"

// The passing path is a single test of the condition. Everything else, the
// location, the stringified expression and the extra arguments, is only
// evaluated inside the branch that calls the cold failure function.
//
// VERIFY is an expression (like assert), so it can be used in constexpr
// functions and comma expressions.

#define VERIFY_NATIVE_1(condition)                                       \
    (VERIFY_LIKELY(condition)                                            \
         ? static_cast<void>(0)                                          \
         : ::verify::detail::verify_failed(__FILE__, __LINE__, __func__, \
                                           #condition, nullptr))

#define VERIFY_NATIVE_N(condition, ...)                                       \
    (VERIFY_LIKELY(condition) ? static_cast<void>(0)                          \
                              : ::verify::detail::verify_failed(              \
                                    __FILE__, __LINE__, __func__, #condition, \
                                    #__VA_ARGS__, __VA_ARGS__))

#define VERIFY_NATIVE_2(condition, ...) VERIFY_NATIVE_N(condition, __VA_ARGS__)

#define VERIFY_IMPL(...) \
    __VERIFY_GLUE(__VERIFY_SELECT(VERIFY_NATIVE_, __VA_ARGS__), (__VA_ARGS__))

// The comparison macros evaluate each operand exactly once. They need a named
// local for that, so unlike VERIFY they are statements, not expressions.

#define VERIFY_CMP_IMPL(macro, op, lhs, rhs)                           \
    do                                                                 \
    {                                                                  \
        const auto& verify_lhs_ = (lhs);                               \
        const auto& verify_rhs_ = (rhs);                               \
        if (!VERIFY_LIKELY(verify_lhs_ op verify_rhs_))                \
            ::verify::detail::compare_failed(                          \
                #macro, __FILE__, __LINE__, __func__, #lhs, #op, #rhs, \
                verify_lhs_, verify_rhs_, nullptr);                    \
    } while (false)

#define VERIFY_CMP_IMPL_ARGS(macro, op, lhs, rhs, ...)                 \
    do                                                                 \
    {                                                                  \
        const auto& verify_lhs_ = (lhs);                               \
        const auto& verify_rhs_ = (rhs);                               \
        if (!VERIFY_LIKELY(verify_lhs_ op verify_rhs_))                \
            ::verify::detail::compare_failed(                          \
                #macro, __FILE__, __LINE__, __func__, #lhs, #op, #rhs, \
                verify_lhs_, verify_rhs_, #__VA_ARGS__, __VA_ARGS__);  \
    } while (false)
