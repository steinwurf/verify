// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include <array>
#include <cstdint>
#include <list>
#include <map>
#include <optional>
#include <set>
#include <sstream>
#include <string>
#include <tuple>
#include <unordered_map>
#include <vector>

#include <fmt/format.h>
// These add fmt formatters for ranges, tuples and optional. verify must still
// print those types its own way, the same as in files without these headers.
#include <fmt/ranges.h>
#include <fmt/std.h>
#include <gtest/gtest.h>
#include <stdexcept>
#include <verify/print.hpp>

namespace
{
template <class T>
std::string to_string(const T& value)
{
    std::ostringstream out;
    verify::print(out, value);
    return out.str();
}

struct with_stream
{
    int x;
};

std::ostream& operator<<(std::ostream& out, const with_stream& w)
{
    return out << "with_stream{" << w.x << "}";
}

struct third_party
{
    int a;
    int b;
};

struct both : with_stream
{
};

enum class color : std::uint8_t
{
    red = 1,
    green = 2
};

enum plain
{
    plain_value = 7
};

// Printable only with fmt
struct fmt_only
{
    int x;
};

// Printable with both: operator<< wins
struct fmt_and_stream
{
    int x;
};

std::ostream& operator<<(std::ostream& out, const fmt_and_stream& v)
{
    return out << "stream{" << v.x << "}";
}

enum class fmt_enum
{
    on = 1
};

auto format_as(fmt_enum e)
{
    return e == fmt_enum::on ? "on" : "off";
}

#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
struct fmt_throws
{
};
#endif
}

template <>
struct fmt::formatter<fmt_only> : fmt::formatter<int>
{
    auto format(const fmt_only& v, fmt::format_context& ctx) const
    {
        ctx.advance_to(fmt::format_to(ctx.out(), "fmt_only{{"));
        ctx.advance_to(fmt::formatter<int>::format(v.x, ctx));
        return fmt::format_to(ctx.out(), "}}");
    }
};

template <>
struct fmt::formatter<fmt_and_stream> : fmt::formatter<int>
{
    auto format(const fmt_and_stream& v, fmt::format_context& ctx) const
    {
        return fmt::format_to(ctx.out(), "fmt{{{}}}", v.x);
    }
};

#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
template <>
struct fmt::formatter<fmt_throws> : fmt::formatter<int>
{
    auto format(const fmt_throws&, fmt::format_context&) const
        -> fmt::format_context::iterator
    {
        throw std::runtime_error("boom");
    }
};
#endif

template <>
struct verify::printer<third_party>
{
    static void print(std::ostream& out, const third_party& v)
    {
        out << "third_party{a=" << v.a << ", b=" << v.b << "}";
    }
};

// A printer specialization wins over operator<<
template <>
struct verify::printer<both>
{
    static void print(std::ostream& out, const both& v)
    {
        out << "both{" << v.x << "}";
    }
};

TEST(test_print, builtins)
{
    EXPECT_EQ("true", to_string(true));
    EXPECT_EQ("false", to_string(false));
    EXPECT_EQ("'a'", to_string('a'));
    EXPECT_EQ("'\\n'", to_string('\n'));
    EXPECT_EQ("'\\''", to_string('\''));
    EXPECT_EQ("200", to_string(std::uint8_t{200}));
    EXPECT_EQ("-5", to_string(std::int8_t{-5}));
    EXPECT_EQ("-42", to_string(-42));
    EXPECT_EQ("18446744073709551615", to_string(UINT64_MAX));
    EXPECT_EQ("1.5", to_string(1.5));
    // Full precision, so values that differ do not print the same
    EXPECT_NE(to_string(0.1 + 0.2), to_string(0.3));
    EXPECT_EQ("0.300000012", to_string(0.3f));
}

TEST(test_print, strings)
{
    EXPECT_EQ("\"hello\"", to_string(std::string("hello")));
    EXPECT_EQ("\"view\"", to_string(std::string_view("view")));
    EXPECT_EQ("\"literal\"", to_string("literal"));
    const char* c_string = "c string";
    EXPECT_EQ("\"c string\"", to_string(c_string));
    const char* null_string = nullptr;
    EXPECT_EQ("nullptr", to_string(null_string));
    EXPECT_EQ("\"a\\\"b\\\\c\\n\\x01\"",
              to_string(std::string("a\"b\\c\n\x01")));
}

TEST(test_print, pointers)
{
    int i = 0;
    int* p = &i;
    int* null = nullptr;
    EXPECT_EQ("nullptr", to_string(null));
    EXPECT_EQ("nullptr", to_string(nullptr));
    EXPECT_EQ(0u, to_string(p).find("0x"));
    EXPECT_NE("0x0", to_string(p));
    volatile int vi = 0;
    volatile int* vp = &vi;
    EXPECT_EQ(to_string(static_cast<void*>(const_cast<int*>(vp))),
              to_string(vp));
}

TEST(test_print, enums)
{
    EXPECT_EQ("1", to_string(color::red));
    EXPECT_EQ("7", to_string(plain_value));
}

TEST(test_print, containers)
{
    EXPECT_EQ("[1, 2, 3]", to_string(std::vector<int>{1, 2, 3}));
    EXPECT_EQ("[]", to_string(std::vector<int>{}));
    EXPECT_EQ("[1, 255]", to_string(std::list<std::uint8_t>{1, 255}));
    EXPECT_EQ("['a', 'b']", to_string(std::set<char>{'b', 'a'}));
    EXPECT_EQ("[true, false]", to_string(std::vector<bool>{true, false}));
    EXPECT_EQ("[1.5, 2]", to_string(std::array<double, 2>{1.5, 2}));
    int c_array[3] = {4, 5, 6};
    EXPECT_EQ("[4, 5, 6]", to_string(c_array));
    EXPECT_EQ("{\"foo\": 2, \"test\": 1}",
              to_string(std::map<std::string, int>{{"test", 1}, {"foo", 2}}));
    EXPECT_EQ("{1: [0.5]}",
              to_string(std::unordered_map<int, std::vector<double>>{
                  {1, {0.5}}}));
    EXPECT_EQ("[[\"a\"], []]",
              to_string(std::vector<std::vector<std::string>>{{"a"}, {}}));
}

TEST(test_print, long_containers_are_truncated)
{
    std::vector<int> v(40, 0);
    std::string s = to_string(v);
    EXPECT_NE(std::string::npos, s.find(", ... (8 more)]"));
}

TEST(test_print, tuple_like)
{
    EXPECT_EQ("(1, \"x\")", to_string(std::make_pair(1, std::string("x"))));
    EXPECT_EQ("(1, 2.5, 'c')", to_string(std::make_tuple(1, 2.5, 'c')));
    EXPECT_EQ("()", to_string(std::tuple<>()));
    EXPECT_EQ("nullopt", to_string(std::optional<int>{}));
    EXPECT_EQ("3", to_string(std::optional<int>{3}));
}

TEST(test_print, custom_types)
{
    EXPECT_EQ("with_stream{7}", to_string(with_stream{7}));
    EXPECT_EQ("[with_stream{1}, with_stream{2}]",
              to_string(std::vector<with_stream>{{1}, {2}}));
    EXPECT_EQ("third_party{a=1, b=2}", to_string(third_party{1, 2}));
    EXPECT_EQ("{1: third_party{a=3, b=4}}",
              to_string(std::map<int, third_party>{{1, {3, 4}}}));
    EXPECT_EQ("both{5}", to_string(both{{5}}));
}

TEST(test_print, fmt_formatter)
{
    EXPECT_EQ("fmt_only{3}", to_string(fmt_only{3}));
    EXPECT_EQ("[fmt_only{1}, fmt_only{2}]",
              to_string(std::vector<fmt_only>{{1}, {2}}));
    EXPECT_EQ("{1: fmt_only{4}}",
              to_string(std::map<int, fmt_only>{{1, {4}}}));
    EXPECT_EQ("stream{5}", to_string(fmt_and_stream{5}));
    EXPECT_EQ("on", to_string(fmt_enum::on));
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
    EXPECT_EQ("<fmt error: boom>", to_string(fmt_throws{}));
#endif

    // fmt/ranges.h and fmt/std.h are included above, but these still use
    // verify's own format: fmt would print all 40 elements, and the optional
    // as "optional(3)"
    std::string long_vector = to_string(std::vector<int>(40, 0));
    EXPECT_NE(std::string::npos, long_vector.find(", ... (8 more)]"));
    EXPECT_EQ("3", to_string(std::optional<int>{3}));
}
