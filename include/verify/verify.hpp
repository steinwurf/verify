// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#pragma once

#ifdef STEINWURF_LIBASSERT

#include <libassert/assert.hpp>

// This is needed do to MSVC++ not supporting variadic macros properly
#define EXPAND(x) x

#define DEBUG_VERIFY(...) EXPAND(LIBASSERT_DEBUG_ASSERT(__VA_ARGS__))
// Keep expr parenthesized to avoid precedence warnings
#define VERIFY1(expr) EXPAND(LIBASSERT_ASSERT((expr)))
#define VERIFY2(expr, ...) EXPAND(LIBASSERT_ASSERT((expr), __VA_ARGS__))

// Pick the 65th parameter; supports up to 64 user arguments (bump if needed)
#define VERIFY_GET_MACRO(                                                      \
    _1, _2, _3, _4, _5, _6, _7, _8, _9, _10, _11, _12, _13, _14, _15, _16,     \
    _17, _18, _19, _20, _21, _22, _23, _24, _25, _26, _27, _28, _29, _30, _31, \
    _32, _33, _34, _35, _36, _37, _38, _39, _40, _41, _42, _43, _44, _45, _46, \
    _47, _48, _49, _50, _51, _52, _53, _54, _55, _56, _57, _58, _59, _60, _61, \
    _62, _63, _64, NAME, ...)                                                  \
    NAME

#define VERIFY_DISPATCH(...)                                           \
    VERIFY_GET_MACRO(                                                  \
        __VA_ARGS__, /* 64 times VERIFY2 */                            \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, VERIFY2, \
        VERIFY2, /* fallback when only 1 arg */                        \
        VERIFY1)

#define VERIFY(...) EXPAND(VERIFY_DISPATCH(__VA_ARGS__)(__VA_ARGS__))

#else

#include <cassert>

// Picks only the first argument from __VA_ARGS__ safely
#define ASSERT_CONDITION_IMPL(arg1, ...) arg1
#define ASSERT_CONDITION(...) ASSERT_CONDITION_IMPL(__VA_ARGS__, dummy)

// Use only the first argument (the condition)
#define DEBUG_VERIFY(...) assert((ASSERT_CONDITION(__VA_ARGS__)))
#define VERIFY(...) assert((ASSERT_CONDITION(__VA_ARGS__)))

#endif