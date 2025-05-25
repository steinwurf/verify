// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

// #ifdef NDEBUG
// #define VERIFY(...) ((void)0)
// #else
// // Select the appropriate backend based on the C++ standard and
// configuration. #ifdef STEINWURF_VERIFY_USE_LIBASSERT #include
// "backends/libassert.hpp" #else #include "backends/assert.hpp" #endif //
// STEINWURF_VERIFY_USE_LIBASSERT

// #ifdef _MSC_VER
// #define VERIFY VERIFY_IMPL
// #else
// #define VERIFY(...) VERIFY_IMPL(__VA_ARGS__)
// #endif // _MSC_VER
// #endif // NDEBUG

////////////////////////////////////

#pragma once

#ifdef STEINWURF_LIBASSERT

#include <libassert/assert.hpp>

// This is needed do to MSVC++ not supporting variadic macros properly
#define EXPAND(x) x

#define DEBUG_VERIFY(...) EXPAND(LIBASSERT_DEBUG_ASSERT(__VA_ARGS__))
#define VERIFY(...) EXPAND(LIBASSERT_ASSERT(__VA_ARGS__))

#else

#include <cassert>

// Picks only the first argument from __VA_ARGS__ safely
#define ASSERT_CONDITION_IMPL(arg1, ...) arg1
#define ASSERT_CONDITION(...) ASSERT_CONDITION_IMPL(__VA_ARGS__, dummy)

// Use only the first argument (the condition)
#define DEBUG_VERIFY(...) assert((ASSERT_CONDITION(__VA_ARGS__)))
#define VERIFY(...) assert((ASSERT_CONDITION(__VA_ARGS__)))

#endif