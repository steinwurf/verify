// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include "verify_variadic_overload_macro.hpp"

// Each enabled backend defines:
//
//   VERIFY_IMPL(condition, ...)                       an expression
//   VERIFY_CMP_IMPL(macro, op, lhs, rhs)              a statement
//   VERIFY_CMP_IMPL_ARGS(macro, op, lhs, rhs, ...)    a statement
//
// The backend is selected with:
//
//   STEINWURF_VERIFY_USE_CASSERT     plain assert(), extra arguments ignored
//   (default)                        the native backend

#ifdef NDEBUG

// Disabled: the verifies are removed, like assert() under NDEBUG. Nothing is
// evaluated and the arguments are not compiled.
#define VERIFY_IMPL(...) static_cast<void>(0)
#define VERIFY_CMP_IMPL(macro, op, lhs, rhs) static_cast<void>(0)
#define VERIFY_CMP_IMPL_ARGS(macro, op, lhs, rhs, ...) static_cast<void>(0)

#elif defined(STEINWURF_VERIFY_USE_CASSERT)
#include "backends/assert.hpp"
#define VERIFY_CMP_IMPL(macro, op, lhs, rhs) VERIFY_IMPL((lhs)op(rhs))
#define VERIFY_CMP_IMPL_ARGS(macro, op, lhs, rhs, ...) \
    VERIFY_IMPL((lhs)op(rhs), __VA_ARGS__)

#else
#include "backends/native.hpp"
#endif

#ifdef _MSC_VER
#define VERIFY VERIFY_IMPL
#else
#define VERIFY(...) VERIFY_IMPL(__VA_ARGS__)
#endif

// VERIFY_EQ(lhs, rhs, ...) and friends: like VERIFY(lhs == rhs, ...), but
// each operand is evaluated once and both values are printed on failure.
// They accept the same optional message and extra values as VERIFY.
#define VERIFY_EQ(...) __VERIFY_CMP(VERIFY_EQ, ==, __VA_ARGS__)
#define VERIFY_NE(...) __VERIFY_CMP(VERIFY_NE, !=, __VA_ARGS__)
#define VERIFY_LT(...) __VERIFY_CMP(VERIFY_LT, <, __VA_ARGS__)
#define VERIFY_LE(...) __VERIFY_CMP(VERIFY_LE, <=, __VA_ARGS__)
#define VERIFY_GT(...) __VERIFY_CMP(VERIFY_GT, >, __VA_ARGS__)
#define VERIFY_GE(...) __VERIFY_CMP(VERIFY_GE, >=, __VA_ARGS__)

// A separate glue macro from the one VERIFY_IMPL uses: the cassert backend
// expands comparisons into VERIFY_IMPL, and a macro is not expanded again
// inside its own expansion.
#define __VERIFY_CMP_GLUE(x, y) x y
#define __VERIFY_CMP(macro, op, ...)                               \
    __VERIFY_CMP_GLUE(__VERIFY_SELECT(__VERIFY_CMP_, __VA_ARGS__), \
                      (macro, op, __VA_ARGS__))
#define __VERIFY_CMP_1(macro, op, lhs) \
    static_assert(sizeof(#lhs) == 0, #macro " needs two values to compare")
#define __VERIFY_CMP_2(macro, op, lhs, rhs) VERIFY_CMP_IMPL(macro, op, lhs, rhs)
#define __VERIFY_CMP_N(macro, op, lhs, rhs, ...) \
    VERIFY_CMP_IMPL_ARGS(macro, op, lhs, rhs, __VA_ARGS__)

// VERIFY_DEBUG: VERIFY when WURFDEBUG is defined, otherwise removed (like
// VERIFY under NDEBUG).
#if defined(WURFDEBUG)
#define VERIFY_DEBUG(...) VERIFY(__VA_ARGS__)
#else
#define VERIFY_DEBUG(...) static_cast<void>(0)
#endif
