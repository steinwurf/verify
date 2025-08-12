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

// Count macro arguments (works on GCC/Clang/MSVC in C++11)
#define VA_NARGS_IMPL(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10,N,...) N
#define VA_NARGS(...) VA_NARGS_IMPL(__VA_ARGS__,10,9,8,7,6,5,4,3,2,1,0)

// Turn a number into a macro name VERIFY_<N>
#define VERIFY_DISPATCH(N) VERIFY_DISPATCH_(N)
#define VERIFY_DISPATCH_(N) VERIFY_##N

// Public macro
#define VERIFY(...) EXPAND(VERIFY_DISPATCH(VA_NARGS(__VA_ARGS__))(__VA_ARGS__))

// 1-arg form: no variadic use, so no empty-VA trouble, and condition is parenthesized
#define VERIFY_1(cond) \
    EXPAND(LIBASSERT_ASSERT((cond)))

// 2+ args all forward to the same helper
#define VERIFY_2(...) VERIFY_2PLUS(__VA_ARGS__)
#define VERIFY_3(...) VERIFY_2PLUS(__VA_ARGS__)
#define VERIFY_4(...) VERIFY_2PLUS(__VA_ARGS__)
#define VERIFY_5(...) VERIFY_2PLUS(__VA_ARGS__)
#define VERIFY_6(...) VERIFY_2PLUS(__VA_ARGS__)
#define VERIFY_7(...) VERIFY_2PLUS(__VA_ARGS__)
#define VERIFY_8(...) VERIFY_2PLUS(__VA_ARGS__)
#define VERIFY_9(...) VERIFY_2PLUS(__VA_ARGS__)
#define VERIFY_10(...) VERIFY_2PLUS(__VA_ARGS__)

// 2+ helper: parenthesize the condition; forward message/vars unchanged
#define VERIFY_2PLUS(cond, ...) \
    EXPAND(LIBASSERT_ASSERT((cond), __VA_ARGS__))


#else

#include <cassert>

// Picks only the first argument from __VA_ARGS__ safely
#define ASSERT_CONDITION_IMPL(arg1, ...) arg1
#define ASSERT_CONDITION(...) ASSERT_CONDITION_IMPL(__VA_ARGS__, dummy)

// Use only the first argument (the condition)
#define DEBUG_VERIFY(...) assert((ASSERT_CONDITION(__VA_ARGS__)))
#define VERIFY(...) assert((ASSERT_CONDITION(__VA_ARGS__)))

#endif