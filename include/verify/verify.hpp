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
#define VERIFY(expr, ...) EXPAND(LIBASSERT_ASSERT((expr), __VA_ARGS__))

#else

#include <cassert>

// Picks only the first argument from __VA_ARGS__ safely
#define ASSERT_CONDITION_IMPL(arg1, ...) arg1
#define ASSERT_CONDITION(...) ASSERT_CONDITION_IMPL(__VA_ARGS__, dummy)

// Use only the first argument (the condition)
#define DEBUG_VERIFY(...) assert((ASSERT_CONDITION(__VA_ARGS__)))
#define VERIFY(...) assert((ASSERT_CONDITION(__VA_ARGS__)))

#endif