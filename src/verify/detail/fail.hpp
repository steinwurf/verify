// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstddef>
#include <iosfwd>
#include <string_view>
#include <type_traits>

#include "../print.hpp"

#if defined(__GNUC__) || defined(__clang__)
#define VERIFY_LIKELY(x) __builtin_expect(static_cast<bool>(x), 1)
#define VERIFY_COLD __attribute__((cold, noinline))
#define VERIFY_ALWAYS_INLINE __attribute__((always_inline)) inline
#elif defined(_MSC_VER)
#define VERIFY_LIKELY(x) static_cast<bool>(x)
#define VERIFY_COLD __declspec(noinline)
#define VERIFY_ALWAYS_INLINE __forceinline
#else
#define VERIFY_LIKELY(x) static_cast<bool>(x)
#define VERIFY_COLD
#define VERIFY_ALWAYS_INLINE inline
#endif

namespace verify
{
namespace detail
{
/// Collects the parts of a failure report and writes it to stderr. Only used
/// on the failure path, so it is implemented out of line in verify.cpp.
class report
{
public:
    report(const char* macro, const char* file, int line, const char* function);

    report(const report&) = delete;
    report& operator=(const report&) = delete;

    /// The checked condition, e.g. "a < b"
    void expression(const char* expression);
    void expression(const char* lhs, const char* op, const char* rhs);

    /// The stringified extra arguments (#__VA_ARGS__) and how many there are
    void argument_names(const char* names, std::size_t count);
    std::string_view argument_name(std::size_t index) const;

    /// Whether extra argument `index` was written as a string literal, which
    /// makes it a message rather than a value to print.
    bool is_literal(std::size_t index) const;

    /// A cleared stream to print the next value or message into
    std::ostream& scratch();

    /// Record the scratch content as a message line
    void add_message();

    /// Record the scratch content as "name = value". When skip_if_same is set
    /// and the name is a literal (`16u` in VERIFY_EQ(size, 16u)) or reads
    /// exactly like the value, the line is left out.
    void add_value(std::string_view name, bool skip_if_same = false);

    /// Write the report to stderr and abort
    [[noreturn]] void fail();

private:
    struct impl;
    impl* m_impl;
};

/// How a value is handed to the cold failure function. Small trivially
/// copyable values are copied: passing a reference would take the address of
/// the caller's variable or member, which can force it out of registers on the
/// hot path (this is what made the libassert backend slow).
template <class T>
using pass_t =
    std::conditional_t<std::is_trivially_copyable_v<T> && !std::is_array_v<T> &&
                           sizeof(T) <= 2 * sizeof(void*),
                       T, const T&>;

template <class T>
constexpr bool is_char_array_v =
    std::is_array_v<T> &&
    std::is_same_v<std::remove_cv_t<std::remove_extent_t<T>>, char>;

template <class T>
void add_argument(report& r, std::size_t index, const T& value)
{
    if constexpr (is_char_array_v<T>)
    {
        if (r.is_literal(index))
        {
            r.scratch() << value;
            r.add_message();
            return;
        }
    }
    verify::print(r.scratch(), value);
    r.add_value(r.argument_name(index));
}

template <class... Args>
[[noreturn]] VERIFY_COLD void fail(const char* file, int line,
                                   const char* function, const char* expression,
                                   const char* names, pass_t<Args>... args)
{
    report r("VERIFY", file, line, function);
    r.expression(expression);
    r.argument_names(names, sizeof...(Args));
    std::size_t index = 0;
    (add_argument<Args>(r, index++, args), ...);
    (void)index;
    r.fail();
}

template <class A, class B, class... Args>
[[noreturn]] VERIFY_COLD void
fail_compare(const char* macro, const char* file, int line,
             const char* function, const char* lhs_name, const char* op,
             const char* rhs_name, pass_t<A> lhs, pass_t<B> rhs,
             const char* names, pass_t<Args>... args)
{
    report r(macro, file, line, function);
    r.expression(lhs_name, op, rhs_name);
    verify::print(r.scratch(), static_cast<const A&>(lhs));
    r.add_value(lhs_name, true);
    verify::print(r.scratch(), static_cast<const B&>(rhs));
    r.add_value(rhs_name, true);
    r.argument_names(names, sizeof...(Args));
    std::size_t index = 0;
    (add_argument<Args>(r, index++, args), ...);
    (void)index;
    r.fail();
}

// The macros call these. They only deduce the argument types and forward to
// the cold functions; being always inlined, the references here are resolved
// before the optimizer decides what has to live in memory.
template <class... Args>
[[noreturn]] VERIFY_ALWAYS_INLINE void
verify_failed(const char* file, int line, const char* function,
              const char* expression, const char* names, const Args&... args)
{
    fail<Args...>(file, line, function, expression, names, args...);
}

template <class A, class B, class... Args>
[[noreturn]] VERIFY_ALWAYS_INLINE void
compare_failed(const char* macro, const char* file, int line,
               const char* function, const char* lhs_name, const char* op,
               const char* rhs_name, const A& lhs, const B& rhs,
               const char* names, const Args&... args)
{
    fail_compare<A, B, Args...>(macro, file, line, function, lhs_name, op,
                                rhs_name, lhs, rhs, names, args...);
}

/// Used when VERIFY is disabled (NDEBUG, or VERIFY_DEBUG in release): called
/// in a branch that is never taken, so nothing is evaluated, but the condition
/// must still compile, every value must be printable, and variables used only
/// in verifies do not trigger unused-variable warnings.
template <class Condition, class... Args>
constexpr int unused(const Condition&, const Args&...)
{
    static_assert(std::is_constructible_v<bool, const Condition&>,
                  "The VERIFY condition must be convertible to bool");
    (static_cast<void>(&verify::print<Args>), ...);
    return 0;
}
}
}
