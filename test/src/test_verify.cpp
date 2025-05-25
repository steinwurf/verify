// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <verify/verify.hpp>

#include "is_running_valgrind.hpp"

TEST(test_verify, simple)
{
    int a = 42;
    VERIFY(a == 42);
    a = 21;
    VERIFY(a == 21);
    VERIFY(a == 21);
}

TEST(test_verify, complex)
{
    std::map<std::string, int> example_map{{"test", 1}, {"foo", 2}};
    auto it = example_map.find("foo");
    VERIFY(it != example_map.cend(), "expected key not found", example_map,
           "hello", "world");
}

void trigger_verify()
{
    std::map<std::string, int> example_map{{"test", 1}, {"foo", 2}};
    auto it = example_map.find("bar");
    VERIFY(it == example_map.cend(), "expected key not found", example_map,
           "hello", "world");
}

TEST(test_verify, failing)
{
#ifdef NDEBUG
    // TODO: Do nothing, tests rely on NDEBUG.
    GTEST_SKIP();
#else

    if (!is_running_valgrind())
    {
        // Only run if not under Valgrind -it will fail when using EXPECT_DEATH
        EXPECT_DEATH(trigger_verify(), "expected key not found");
    }
#endif
}
