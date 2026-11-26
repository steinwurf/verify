// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstddef>
#include <cstdint>
#include <iterator>
#include <limits>
#include <optional>
#include <ostream>
#include <string>
#include <string_view>
#include <tuple>
#include <type_traits>
#include <utility>

#include <fmt/base.h>

namespace verify
{
/// Customization point for printing a value in a failed VERIFY.
///
/// Types with an ``operator<<`` or an ``fmt::formatter`` are printed
/// automatically. For other types, or to print a type differently in verify
/// output only, specialize:
///
///     template <>
///     struct verify::printer<my::type>
///     {
///         static void print(std::ostream& out, const my::type& value)
///         {
///             out << "my::type{" << value.x << "}";
///         }
///     };
///
/// The specialization must be visible wherever the VERIFY using the type is.
/// Inside print() use verify::print(out, member) to print members with the
/// same rules, e.g. containers of other printable types.
template <class T, class Enable = void>
struct printer;

/// Print value the way a failed VERIFY prints it. A type that cannot be printed
/// is a compile error (static_assert), even in builds where VERIFY is disabled.
template <class T>
void print(std::ostream& out, const T& value);

namespace detail
{
template <class...>
struct always_false : std::false_type
{
};

template <class T, class = void>
struct has_printer : std::false_type
{
};

template <class T>
struct has_printer<
    T, std::void_t<decltype(printer<T>::print(std::declval<std::ostream&>(),
                                              std::declval<const T&>()))>>
    : std::true_type
{
};

template <class T, class = void>
struct has_ostream : std::false_type
{
};

template <class T>
struct has_ostream<T, std::void_t<decltype(std::declval<std::ostream&>()
                                           << std::declval<const T&>())>>
    : std::true_type
{
};

template <class T, class = void>
struct is_range : std::false_type
{
};

template <class T>
struct is_range<T, std::void_t<decltype(std::begin(std::declval<const T&>())),
                               decltype(std::end(std::declval<const T&>()))>>
    : std::true_type
{
};

template <class T, class = void>
struct is_map : std::false_type
{
};

template <class T>
struct is_map<T, std::void_t<typename T::key_type, typename T::mapped_type>>
    : std::true_type
{
};

// The type elements of a range are printed as. Uses value_type when there is
// one, so proxy references (std::vector<bool>) print as their value.
template <class T, class = void>
struct range_element
{
    using type = std::decay_t<decltype(*std::begin(std::declval<const T&>()))>;
};

template <class T>
struct range_element<T, std::void_t<typename T::value_type>>
{
    using type = typename T::value_type;
};

template <class T>
struct is_pair : std::false_type
{
};

template <class A, class B>
struct is_pair<std::pair<A, B>> : std::true_type
{
};

template <class T>
struct is_tuple : std::false_type
{
};

template <class... Ts>
struct is_tuple<std::tuple<Ts...>> : std::true_type
{
};

template <class T>
struct is_optional : std::false_type
{
};

template <class T>
struct is_optional<std::optional<T>> : std::true_type
{
};

/// Whether T is printed with its fmt::formatter. Ranges, tuples and optionals
/// always use verify's own printing: fmt only formats them in files that
/// include fmt/ranges.h or fmt/std.h, and the same type must print the same way
/// everywhere.
///
/// fmt::is_formattable is only evaluated for the other types: for a range of
/// an unformattable type, fmt's range formatter is a hard error with GCC 8
/// rather than "not formattable". fmt::is_formattable is an alias, which is
/// evaluated as soon as it is named, so it is wrapped in a class template that
/// std::conjunction only instantiates when the checks before it are true.
template <class T>
struct is_fmt_formattable : std::bool_constant<fmt::is_formattable<T>::value>
{
};

template <class T>
constexpr bool use_fmt_v =
    std::conjunction_v<std::negation<is_range<T>>, std::negation<is_pair<T>>,
                       std::negation<is_tuple<T>>,
                       std::negation<is_optional<T>>, is_fmt_formattable<T>>;

/// Write value with its fmt::formatter. Out of line (verify.cpp), so only the
/// type-erased argument is built where the value is printed.
void print_formatted(std::ostream& out, fmt::format_args args);

/// Containers print at most this many elements
constexpr std::size_t max_elements = 32;

/// char pointers print at most this many characters. The length behind the
/// pointer is unknown, and a char* often points at binary data rather than a
/// NUL-terminated string, so reading is bounded instead of going to the NUL.
constexpr std::size_t max_c_string = 64;

inline void print_quoted(std::ostream& out, std::string_view text);

inline void print_c_string(std::ostream& out, const char* s)
{
    std::size_t n = 0;
    while (n < max_c_string && s[n] != '\0')
        ++n;
    print_quoted(out, std::string_view(s, n));
    if (n == max_c_string)
        out << "...";
}

inline void print_escaped(std::ostream& out, char c, char quote)
{
    static const char hex[] = "0123456789abcdef";
    switch (c)
    {
    case '\n':
        out << "\\n";
        break;
    case '\t':
        out << "\\t";
        break;
    case '\r':
        out << "\\r";
        break;
    case '\0':
        out << "\\0";
        break;
    case '\\':
        out << "\\\\";
        break;
    default:
    {
        auto u = static_cast<unsigned char>(c);
        if (c == quote)
            out << '\\' << c;
        else if (u < 0x20 || u == 0x7f)
            out << "\\x" << hex[u >> 4] << hex[u & 0xf];
        else
            out << c;
    }
    }
}

inline void print_quoted(std::ostream& out, std::string_view text)
{
    out << '"';
    for (char c : text)
        print_escaped(out, c, '"');
    out << '"';
}

inline void print_char(std::ostream& out, char c)
{
    out << '\'';
    print_escaped(out, c, '\'');
    out << '\'';
}

template <class Range>
void print_range(std::ostream& out, const Range& range)
{
    using element = typename range_element<Range>::type;
    constexpr bool map = is_map<Range>::value;

    out << (map ? '{' : '[');
    std::size_t n = 0;
    for (auto it = std::begin(range); it != std::end(range); ++it, ++n)
    {
        if (n == max_elements)
        {
            auto total = std::distance(std::begin(range), std::end(range));
            out << ", ... (" << static_cast<std::size_t>(total) - n << " more)";
            break;
        }
        if (n != 0)
            out << ", ";
        if constexpr (map)
        {
            const auto& kv = *it;
            verify::print(out, kv.first);
            out << ": ";
            verify::print(out, kv.second);
        }
        else
        {
            const element& e = *it;
            verify::print(out, e);
        }
    }
    out << (map ? '}' : ']');
}

template <class Tuple, std::size_t... I>
void print_tuple(std::ostream& out, const Tuple& t, std::index_sequence<I...>)
{
    out << '(';
    ((out << (I == 0 ? "" : ", "), verify::print(out, std::get<I>(t))), ...);
    out << ')';
}
}

template <class T>
void print(std::ostream& out, const T& v)
{
    // The order matters: explicit customization wins, then built-in rules for
    // types whose operator<< is unhelpful (bool, char types, pointers, arrays),
    // then the type's own operator<<, then its fmt::formatter, then the
    // structural fallbacks.
    if constexpr (detail::has_printer<T>::value)
    {
        printer<T>::print(out, v);
    }
    else if constexpr (std::is_same_v<T, bool>)
    {
        out << (v ? "true" : "false");
    }
    else if constexpr (std::is_same_v<T, char>)
    {
        detail::print_char(out, v);
    }
    else if constexpr (std::is_same_v<T, signed char> ||
                       std::is_same_v<T, unsigned char>)
    {
        // int8_t / uint8_t are numbers, not characters
        out << static_cast<int>(v);
    }
    else if constexpr (std::is_floating_point_v<T>)
    {
        auto precision = out.precision(std::numeric_limits<T>::max_digits10);
        out << v;
        out.precision(precision);
    }
    else if constexpr (std::is_arithmetic_v<T>)
    {
        out << v;
    }
    else if constexpr (std::is_same_v<T, std::string> ||
                       std::is_same_v<T, std::string_view>)
    {
        detail::print_quoted(out, v);
    }
    else if constexpr (std::is_array_v<T> &&
                       std::is_same_v<std::remove_cv_t<std::remove_extent_t<T>>,
                                      char>)
    {
        // A char array (usually a string literal): print up to the first NUL
        detail::print_quoted(out, std::string_view(v));
    }
    else if constexpr (std::is_same_v<T, const char*> ||
                       std::is_same_v<T, char*>)
    {
        if (v == nullptr)
            out << "nullptr";
        else
            detail::print_c_string(out, v);
    }
    else if constexpr (std::is_null_pointer_v<T>)
    {
        out << "nullptr";
    }
    else if constexpr (std::is_pointer_v<T>)
    {
        if (v == nullptr)
        {
            out << "nullptr";
        }
        else
        {
            // Not via void*: ostream has no overload for pointers to volatile
            // (before C++23) and would print them as bool.
            auto flags = out.flags();
            out << "0x" << std::hex << reinterpret_cast<std::uintptr_t>(v);
            out.flags(flags);
        }
    }
    else if constexpr (std::is_array_v<T>)
    {
        detail::print_range(out, v);
    }
    else if constexpr (detail::has_ostream<T>::value)
    {
        out << v;
    }
    else if constexpr (detail::use_fmt_v<T>)
    {
        // After operator<<: a type with both must print the same way whether
        // or not fmt/std.h (or another header adding a formatter) is included
        detail::print_formatted(out, fmt::make_format_args(v));
    }
    else if constexpr (std::is_enum_v<T>)
    {
        verify::print(out, static_cast<std::underlying_type_t<T>>(v));
    }
    else if constexpr (detail::is_pair<T>::value || detail::is_tuple<T>::value)
    {
        detail::print_tuple(out, v,
                            std::make_index_sequence<std::tuple_size_v<T>>{});
    }
    else if constexpr (detail::is_optional<T>::value)
    {
        if (v)
            verify::print(out, *v);
        else
            out << "nullopt";
    }
    else if constexpr (detail::is_range<T>::value)
    {
        detail::print_range(out, v);
    }
    else
    {
        static_assert(detail::always_false<T>::value,
                      "VERIFY cannot print a value of type T (see the "
                      "template argument in the error above). Give the type "
                      "an operator<<(std::ostream&, const T&) or an "
                      "fmt::formatter<T>, or specialize "
                      "verify::printer<T> with a static "
                      "print(std::ostream&, const T&) function.");
    }
}
}
