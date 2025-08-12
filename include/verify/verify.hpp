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
// If you already have EXPAND, keep using it. Otherwise:
#define EXPAND(x) x

// Choose VERIFY_WITH_MSG when there are >= 2 args, else VERIFY_NO_MSG.
// Add more placeholders if you might pass >10 total args.
#define VERIFY_CHOOSER(_1,_2,_3,_4,_5,_6,_7,_8,_9,_10, NAME, ...) NAME

#define VERIFY(...) \
    EXPAND(VERIFY_CHOOSER(__VA_ARGS__, \
        VERIFY_WITH_MSG, VERIFY_WITH_MSG, VERIFY_WITH_MSG, VERIFY_WITH_MSG, \
        VERIFY_WITH_MSG, VERIFY_WITH_MSG, VERIFY_WITH_MSG, VERIFY_WITH_MSG, \
        VERIFY_WITH_MSG, /* if only 1 arg, fall through to: */ \
        VERIFY_NO_MSG)(__VA_ARGS__))

// Force parentheses around the condition to fix operator precedence issues.
#define VERIFY_NO_MSG(cond) \
    EXPAND(LIBASSERT_ASSERT((cond)))

#define VERIFY_WITH_MSG(cond, ...) \
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