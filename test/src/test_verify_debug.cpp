// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <gtest/gtest.h>
#include <map>
#include <string>
#include <verify/verify.hpp>

#include "is_running_valgrind.hpp"

TEST(test_verify_debug, simple)
{
    int a = 42;
    VERIFY_DEBUG(a == 42, "OK");
    VERIFY_DEBUG(a == 42, "OK", "OK");
    VERIFY_DEBUG(a == 42, "OK", "OK", "OK");
    VERIFY_DEBUG(a == 42, "OK", "OK", "OK", "OK");
    VERIFY_DEBUG(a == 42, "OK", "OK", "OK", "OK", "OK");
    a = 21;
    VERIFY_DEBUG(a == 21);
    VERIFY_DEBUG(a == 21);
    (void)a;
}

TEST(test_verify_debug, complex)
{
    std::map<std::string, int> example_map{{"test", 1}, {"foo", 2}};
    auto it = example_map.find("foo");
    VERIFY_DEBUG(it != example_map.cend(), "expected key not found",
                 example_map, "hello", "world");
    (void)it;
}

namespace
{
void trigger_verify()
{
    std::map<std::string, int> example_map{{"test", 1}, {"foo", 2}};
    auto it = example_map.find("bar");
    VERIFY_DEBUG(it != example_map.cend(), "expected key not found",
                 example_map, "hello", "world");
    (void)it;
}
}

TEST(test_verify_debug, failing)
{
#ifdef NDEBUG
    // TODO: Do nothing, tests rely on NDEBUG.
    GTEST_SKIP();
#else

#if defined(WURFDEBUG)
    if (!is_running_valgrind())
    {
        // Only run if not under Valgrind -it will fail when using EXPECT_DEATH
        EXPECT_DEATH(trigger_verify(), "expected key not found");
    }
#elif defined(WURFRELEASE)
    trigger_verify(); // Should not trigger
#endif

#endif
}
