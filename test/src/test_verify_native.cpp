// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <cstdlib>
#include <map>
#include <string>
#include <utility>
#include <vector>

#include <gtest/gtest.h>
#include <verify/verify.hpp>

#include "is_running_valgrind.hpp"

// The comparison macros work with every backend; the exact failure output is
// only checked for the native backend (the default).

namespace
{
int g_calls = 0;

int count_call(int value)
{
    ++g_calls;
    return value;
}

struct unprintable_but_listed
{
};
}

TEST(test_verify_compare, passing)
{
    int a = 1;
    int b = 2;
    VERIFY_EQ(a, 1);
    VERIFY_NE(a, b);
    VERIFY_LT(a, b);
    VERIFY_LE(a, a);
    VERIFY_GT(b, a);
    VERIFY_GE(b, b, "with a message");
    VERIFY_EQ(a + 1, b, "with a message and values", a, b);

    std::vector<int> v{1, 2, 3};
    VERIFY_EQ(v.size(), 3u);
    VERIFY_EQ(v, (std::vector<int>{1, 2, 3}));

    int* null = nullptr;
    VERIFY_EQ(null, nullptr);
}

TEST(test_verify_compare, operands_evaluated_once)
{
    g_calls = 0;
    VERIFY_EQ(count_call(1), count_call(1));
    VERIFY_LT(count_call(1), count_call(2), "message", 42);
#ifdef NDEBUG
    EXPECT_EQ(0, g_calls);
#else
    EXPECT_EQ(4, g_calls);
#endif
}

TEST(test_verify_compare, usable_as_single_statement)
{
    int a = 1;
    if (a == 1)
        VERIFY_EQ(a, 1);
    else
        VERIFY_NE(a, 1);
}

#if !defined(NDEBUG) && !defined(STEINWURF_VERIFY_USE_CASSERT)

namespace
{
constexpr int checked_square(int x)
{
    VERIFY(x >= 0, "negative input", x);
    return x * x;
}

// VERIFY is an expression and works in constant evaluation while it passes
static_assert(checked_square(3) == 9, "");

void fail_eq()
{
    int size = 17;
    int expected_size = 16;
    VERIFY_EQ(size, expected_size, "header mismatch");
}

void fail_lt_expression()
{
    int index = 7;
    int n = 2;
    VERIFY_LT(index, 2 * n, "index out of range");
}

void fail_eq_suffixed_literal()
{
    unsigned long size = 3;
    VERIFY_EQ(size, 16ul);
}

void fail_with_values()
{
    std::map<std::string, int> m{{"a", 1}};
    std::pair<int, int> p{1, 2};
    VERIFY(m.count("b") == 1, "missing key", m, std::pair<int, int>(3, 4), p);
}

void fail_plain()
{
    int x = 3;
    VERIFY(x == 4);
}
}

#if defined(__has_include)
#if __has_include(<execinfo.h>) && __has_include(<dlfcn.h>) && \
    __has_include(<cxxabi.h>)
#define TEST_HAS_STACK_TRACE 1
#if defined(__ELF__) && __has_include(<spawn.h>) && \
    __has_include(<sys/wait.h>) && __has_include(<fcntl.h>)
#define TEST_HAS_SYMBOLIZER 1
#endif
#endif
#endif

// Death test regexes. gtest on Windows only has "simple" regular expressions:
// no groups, alternatives or bracket expressions.
#if defined(GTEST_USES_SIMPLE_RE) && GTEST_USES_SIMPLE_RE
#define TEST_RE_NUMBER "\\d+"
// The report ends after the last value line
#define TEST_RE_END "\n"
#else
#define TEST_RE_NUMBER "[0-9]+"
// The last value line is followed by the stack trace or the end of output
#define TEST_RE_END "\n(stack trace:\n|$)"
#endif

#if defined(TEST_HAS_STACK_TRACE)
// Not inlined, so it is its own frame in the stack trace
#if defined(__GNUC__) || defined(__clang__)
__attribute__((noinline))
#endif
void verify_stack_trace_caller(int x)
{
    VERIFY(x == 4);
}
#endif

TEST(test_verify_native, extra_arguments_not_evaluated_while_passing)
{
    g_calls = 0;
    VERIFY(true, "message", count_call(1), count_call(2));
    EXPECT_EQ(0, g_calls);
}

TEST(test_verify_native, usable_as_expression)
{
    int x = 1;
    int y = (VERIFY(x == 1), x + 1);
    EXPECT_EQ(2, y);
}

TEST(test_verify_native, failure_output)
{
    if (is_running_valgrind())
        GTEST_SKIP() << "EXPECT_DEATH does not work under Valgrind";

    EXPECT_DEATH(fail_eq(), "VERIFY_EQ failed in fail_eq\\(\\): size == "
                            "expected_size\n"
                            "  header mismatch\n"
                            "  size          = 17\n"
                            "  expected_size = 16\n");

    // A literal operand is not repeated as "16ul = 16"...
    EXPECT_DEATH(fail_eq_suffixed_literal(),
                 "VERIFY_EQ failed in fail_eq_suffixed_literal\\(\\): "
                 "size == 16ul\n"
                 "  size = 3" TEST_RE_END);
    // ...but an expression that starts with a literal is printed
    EXPECT_DEATH(
        fail_lt_expression(),
        "VERIFY_LT failed in fail_lt_expression\\(\\): index < 2 \\* n\n"
        "  index out of range\n"
        "  index = 7\n"
        "  2 \\* n = 4" TEST_RE_END);

    // Names are split correctly around commas inside brackets
    EXPECT_DEATH(fail_with_values(),
                 "VERIFY failed in fail_with_values\\(\\): "
                 "m.count\\(\"b\"\\) == 1\n"
                 "  missing key\n"
                 "  m                         = \\{\"a\": 1\\}\n"
                 "  std::pair<int, int>\\(3, 4\\) = \\(3, 4\\)\n"
                 "  p                         = \\(1, 2\\)\n");

#if defined(TEST_HAS_STACK_TRACE)
    // The trace starts at the function with the failing VERIFY. Its frame may
    // have no name (GCC moves the failure branch to a local .cold clone), but
    // the next one is the test body.
    EXPECT_DEATH(verify_stack_trace_caller(3),
                 "VERIFY failed in verify_stack_trace_caller\\(\\): x == 4\n"
                 "stack trace:\n"
                 "  #0  [^\n]*\\(\\+0x[0-9a-f]+\\)[^\n]*\n"
                 "(        at [^\n]*\n)*"
                 "  #1  [^\n]*\\(\\+0x[0-9a-f]+\\) "
                 "test_verify_native_failure_output_Test::TestBody\\(\\)\n");
#endif

#if defined(TEST_HAS_SYMBOLIZER)
    // With addr2line and debug info (the tests are built with -g), frame #0
    // names the line of the failing VERIFY, without verify's own functions
    // that were inlined there
    if (std::system("command -v addr2line > /dev/null 2>&1") == 0)
    {
        EXPECT_DEATH(verify_stack_trace_caller(3),
                     "stack trace:\n"
                     "  #0  [^\n]*\\(\\+0x[0-9a-f]+\\)[^\n]*\n"
                     "        at [^\n]*test_verify_native\\.cpp:[0-9]+\n"
                     "  #1  ");
    }

    // VERIFY_SYMBOLIZE=0 turns it off: module and offset only
    EXPECT_DEATH(
        {
            ::setenv("VERIFY_SYMBOLIZE", "0", 1);
            verify_stack_trace_caller(3);
        },
        "stack trace:\n"
        "  #0  [^\n]*\\(\\+0x[0-9a-f]+\\)[^\n]*\n"
        "  #1  [^\n]*\\(\\+0x[0-9a-f]+\\) "
        "test_verify_native_failure_output_Test::TestBody\\(\\)\n"
        "  #2  ");
#endif

    EXPECT_DEATH(fail_plain(),
                 "test_verify_native.cpp:" TEST_RE_NUMBER ": VERIFY failed "
                 "in fail_plain\\(\\): x == 4" TEST_RE_END);
}

#endif
