// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// VERIFY with NDEBUG: the verifies are removed and nothing is evaluated.
#ifndef NDEBUG
#define NDEBUG
#endif

#include <gtest/gtest.h>
#include <verify/verify.hpp>

namespace
{
int g_calls = 0;

// Only called inside verifies, which are removed under NDEBUG
[[maybe_unused]] int count_call(int value)
{
    ++g_calls;
    return value;
}
}

TEST(test_verify_ndebug, nothing_is_evaluated)
{
    g_calls = 0;
    VERIFY(count_call(1) == 2);
    VERIFY(count_call(1) == 2, "message", count_call(3));
    VERIFY_EQ(count_call(1), 2);
    VERIFY_LT(count_call(3), 2, "message", count_call(4));
    VERIFY_DEBUG(count_call(1) == 2);
    EXPECT_EQ(0, g_calls);
}
