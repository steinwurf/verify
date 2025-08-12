// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

// ----------------------------------------------------
// Variadic Function Helper Macro
// ----------------------------------------------------
// Inspired by: https://stackoverflow.com/a/26408195
//              https://stackoverflow.com/a/24028231

#define __VERIFY_GLUE(x, y) x y

#define __VERIFY_RETURN_ARG_COUNT(                                             \
    _1_, _2_, _3_, _4_, _5_, _6_, _7_, _8_, _9_, _10_, _11_, _12_, _13_, _14_, \
    _15_, _16_, _17_, _18_, _19_, _20_, _21_, _22_, _23_, _24_, _25_, _26_,    \
    _27_, _28_, _29_, _30_, _31_, _32_, count, ...)                            \
    count
#define __VERIFY_EXPAND_ARGS(args) __VERIFY_RETURN_ARG_COUNT args
#define __VERIFY_COUNT_ARGS_MAX32(...)                                         \
    __VERIFY_EXPAND_ARGS((__VA_ARGS__, 32, 31, 30, 29, 28, 27, 26, 25, 24, 23, \
                          22, 21, 20, 19, 18, 17, 16, 15, 14, 13, 12, 11, 10,  \
                          9, 8, 7, 6, 5, 4, 3, 2, 1, 0))

#define __VERIFY_OVERLOAD_MACRO2(name, count) name##count
#define __VERIFY_OVERLOAD_MACRO1(name, count) \
    __VERIFY_OVERLOAD_MACRO2(name, count)
#define __VERIFY_OVERLOAD_MACRO(name, count) \
    __VERIFY_OVERLOAD_MACRO1(name, count)

#define __VERIFY_OVERLOAD(name, ...)                                           \
    __VERIFY_GLUE(                                                             \
        __VERIFY_OVERLOAD_MACRO(name, __VERIFY_COUNT_ARGS_MAX32(__VA_ARGS__)), \
        (__VA_ARGS__))