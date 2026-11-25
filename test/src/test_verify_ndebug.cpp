// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// VERIFY with NDEBUG: nothing is evaluated, but everything is compiled. This
// translation unit is built with -Wall -Wextra -Werror, so it also checks that
// variables used only in verifies do not cause unused-variable warnings.
#ifndef NDEBUG
#define NDEBUG
#endif

#include <algorithm>
#include <map>
#include <string>
#include <vector>

#include <gtest/gtest.h>
#include <verify/verify.hpp>

namespace
{
int g_calls = 0;

int count_call(int value)
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

TEST(test_verify_ndebug, verify_only_variables_are_used)
{
    // Without the compile check these would be unused under NDEBUG
    int only_in_verify = 1;
    std::map<std::string, int> only_in_message{{"a", 1}};
    VERIFY(only_in_verify == 1, "message", only_in_message);

    int only_in_compare = 2;
    VERIFY_GT(only_in_compare, 1);
}

TEST(test_verify_ndebug, lambdas_in_condition)
{
    std::vector<int> v{1, 2, 3};
    VERIFY(std::all_of(v.begin(), v.end(), [](int x) { return x > 0; }));
}
