// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#include "verify.hpp"

#include "detail/fail.hpp"

#include <algorithm>
#include <cctype>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <exception>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

#include <fmt/format.h>

#if defined(__has_include)
#if __has_include(<execinfo.h>) && __has_include(<dlfcn.h>) && \
    __has_include(<cxxabi.h>)
#define VERIFY_HAS_STACK_TRACE 1
#include <cxxabi.h>
#include <dlfcn.h>
#include <execinfo.h>
#if defined(__ELF__) && __has_include(<link.h>)
#include <link.h>
#endif
#endif
#endif

namespace verify
{
namespace detail
{
namespace
{
std::string_view trim(std::string_view s)
{
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.front())))
        s.remove_prefix(1);
    while (!s.empty() && std::isspace(static_cast<unsigned char>(s.back())))
        s.remove_suffix(1);
    return s;
}

// Split the stringified argument list (#__VA_ARGS__) on top-level commas,
// skipping commas inside brackets and string/character literals. With
// `angles` set, <...> also counts as brackets (template arguments); that is
// tried second because `<` is more often a comparison.
std::vector<std::string_view> split_names(std::string_view names, bool angles)
{
    std::vector<std::string_view> result;
    int depth = 0;
    std::size_t start = 0;
    for (std::size_t i = 0; i < names.size(); ++i)
    {
        char c = names[i];
        if (c == '"' ||
            (c == '\'' && (i == 0 || !std::isalnum(static_cast<unsigned char>(
                                         names[i - 1])))))
        {
            // Skip the literal. A ' after an alphanumeric is a digit
            // separator (1'000), not a character literal.
            for (++i; i < names.size() && names[i] != c; ++i)
                if (names[i] == '\\')
                    ++i;
            continue;
        }
        if (c == '(' || c == '[' || c == '{' || (angles && c == '<'))
            ++depth;
        else if (c == ')' || c == ']' || c == '}' || (angles && c == '>'))
            --depth;
        else if (c == ',' && depth == 0)
        {
            result.push_back(trim(names.substr(start, i - start)));
            start = i + 1;
        }
    }
    result.push_back(trim(names.substr(start)));
    return result;
}

// Whether an operand is a single literal token (16u, -1, 1'000, 0x1fp-3, 1.5f,
// 'a', "x", true, nullptr): printing "16u = 16" would add nothing. An
// expression such as `2 * n` is not a literal and is printed.
bool is_literal_operand(std::string_view name)
{
    name = trim(name);
    if (!name.empty() && (name.front() == '-' || name.front() == '+'))
        name = trim(name.substr(1));
    if (name.empty())
        return false;
    if (name == "true" || name == "false" || name == "nullptr")
        return true;

    char first = name.front();
    if (first == '"' || first == '\'')
    {
        // One string or character literal, start to end
        for (std::size_t i = 1; i < name.size(); ++i)
        {
            if (name[i] == '\\')
                ++i;
            else if (name[i] == first)
                return i == name.size() - 1;
        }
        return false;
    }

    if (!std::isdigit(static_cast<unsigned char>(first)) && first != '.')
        return false;
    for (std::size_t i = 0; i < name.size(); ++i)
    {
        char c = name[i];
        if (std::isalnum(static_cast<unsigned char>(c)) || c == '.' ||
            c == '\'')
            continue;
        // Exponent sign: 1e-3, 0x1p+4
        char prev = i > 0 ? name[i - 1] : '\0';
        if ((c == '-' || c == '+') &&
            (prev == 'e' || prev == 'E' || prev == 'p' || prev == 'P'))
            continue;
        return false;
    }
    return true;
}

#if defined(VERIFY_HAS_STACK_TRACE)
/// Quote a path for a POSIX shell, unless it only has characters that need
/// no quoting
std::string shell_quote(std::string_view path)
{
    bool plain = !path.empty();
    for (char c : path)
        if (!std::isalnum(static_cast<unsigned char>(c)) &&
            std::string_view("/._+-").find(c) == std::string_view::npos)
            plain = false;
    if (plain)
        return std::string(path);

    std::string quoted = "'";
    for (char c : path)
    {
        if (c == '\'')
            quoted += "'\\''";
        else
            quoted += c;
    }
    quoted += '\'';
    return quoted;
}

std::string demangle(const char* name)
{
    int status = 0;
    char* demangled = abi::__cxa_demangle(name, nullptr, nullptr, &status);
    if (status != 0 || demangled == nullptr)
        return name;
    std::string result(demangled);
    std::free(demangled);
    return result;
}

// Print the call stack, starting at the function that called the failing
// VERIFY. Only runs on the failure path, so it costs nothing while verifies
// pass. Each frame is printed as `module(+offset)`, which addr2line and
// llvm-symbolizer accept; on ELF systems the matching addr2line commands are
// printed after the trace, ready to copy. Function names
// are only known for symbols in the dynamic symbol table (shared libraries, or
// executables linked with -rdynamic).
void print_stack_trace(void* caller)
{
    void* frames[64];
    int count = ::backtrace(frames, 64);

    // Skip the frames inside verify: everything up to and including the cold
    // failure function that called report::fail(). If that frame cannot be
    // found, print the whole stack.
    int first = 0;
    for (int i = 0; i < count; ++i)
    {
        if (frames[i] == caller)
        {
            first = i + 1;
            break;
        }
    }
    if (first >= count)
        return;

    std::string out = "stack trace:\n";
#if defined(__ELF__)
    // The frames of each module, in the order the modules first appear, for
    // the addr2line commands printed after the trace
    std::vector<std::pair<std::string, std::string>> modules;
#endif
    for (int i = first; i < count; ++i)
    {
        // A return address points after the call; step back into it so the
        // symbolized line is the call itself.
        auto address = reinterpret_cast<std::uintptr_t>(frames[i]) - 1;

        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "  #%-2d ", i - first);
        out += buffer;

        Dl_info info{};
        if (::dladdr(reinterpret_cast<void*>(address), &info) == 0 ||
            info.dli_fname == nullptr || info.dli_fname[0] == '\0')
        {
            std::snprintf(buffer, sizeof(buffer), "0x%jx\n",
                          static_cast<std::uintmax_t>(address));
            out += buffer;
            continue;
        }

        auto base = reinterpret_cast<std::uintptr_t>(info.dli_fbase);
        std::uintptr_t offset = address - base;
#if defined(__ELF__) && __has_include(<link.h>)
        // A non-PIE executable is not relocated: tools want the address
        auto header = static_cast<const ElfW(Ehdr)*>(info.dli_fbase);
        if (header != nullptr && header->e_type == ET_EXEC)
            offset = address;
#endif
        std::snprintf(buffer, sizeof(buffer), "(+0x%jx)",
                      static_cast<std::uintmax_t>(offset));
        out += info.dli_fname;
        out += buffer;
        if (info.dli_sname != nullptr)
        {
            out += ' ';
            out += demangle(info.dli_sname);
        }
        out += '\n';

#if defined(__ELF__)
        std::snprintf(buffer, sizeof(buffer), " 0x%jx",
                      static_cast<std::uintmax_t>(offset));
        auto module =
            std::find_if(modules.begin(), modules.end(), [&](const auto& m)
                         { return m.first == info.dli_fname; });
        if (module == modules.end())
            modules.emplace_back(info.dli_fname, buffer);
        else
            module->second += buffer;
#endif
    }

#if defined(__ELF__)
    // Nothing is run here: these are for the developer to copy, and give
    // function, file and line (including inlined calls) on a build with debug
    // info
    if (!modules.empty())
    {
        out += "file and line (needs debug info):\n";
        for (const auto& [path, offsets] : modules)
        {
            out += "  addr2line -f -C -i -e ";
            out += shell_quote(path);
            out += offsets;
            out += '\n';
        }
    }
#endif
    std::fputs(out.c_str(), stderr);
    std::fflush(stderr);
}
#endif

struct entry
{
    bool is_message;
    std::string name;
    std::string text;
};
}

struct report::impl
{
    const char* macro;
    const char* file;
    int line;
    const char* function;
    std::string expression;
    std::vector<std::string_view> names;
    std::vector<std::string> fallback_names;
    std::vector<entry> lines;
    std::ostringstream scratch;
};

report::report(const char* macro, const char* file, int line,
               const char* function) :
    // Never freed: the process aborts right after the report is written
    m_impl(new impl{macro, file, line, function, {}, {}, {}, {}, {}})
{
}

void report::expression(const char* expression)
{
    m_impl->expression = expression;
}

void report::expression(const char* lhs, const char* op, const char* rhs)
{
    m_impl->expression = std::string(lhs) + " " + op + " " + rhs;
}

void report::argument_names(const char* names, std::size_t count)
{
    if (count == 0 || names == nullptr)
        return;

    auto split = split_names(names, false);
    if (split.size() != count)
        split = split_names(names, true);
    if (split.size() != count)
    {
        // Could not tell the arguments apart; number them instead
        split.clear();
        for (std::size_t i = 0; i < count; ++i)
            m_impl->fallback_names.push_back("#" + std::to_string(i + 1));
        for (const auto& n : m_impl->fallback_names)
            split.push_back(n);
    }
    m_impl->names = std::move(split);
}

std::string_view report::argument_name(std::size_t index) const
{
    return index < m_impl->names.size() ? m_impl->names[index]
                                        : std::string_view("?");
}

bool report::is_literal(std::size_t index) const
{
    std::string_view name = argument_name(index);
    // "...", u8"...", R"(...)", LR"(...)" etc.
    auto quote = name.find('"');
    if (quote == std::string_view::npos || quote > 3)
        return false;
    for (std::size_t i = 0; i < quote; ++i)
        if (std::string_view("u8ULR").find(name[i]) == std::string_view::npos)
            return false;
    return true;
}

std::ostream& report::scratch()
{
    m_impl->scratch.str(std::string());
    m_impl->scratch.clear();
    return m_impl->scratch;
}

void report::print_exception()
{
    std::string what = "<exception while printing";
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
    try
    {
        throw;
    }
    catch (const std::exception& e)
    {
        what += ": ";
        what += e.what();
    }
    catch (...)
    {
    }
#endif
    what += '>';
    scratch() << what;
}

void report::add_message()
{
    m_impl->lines.push_back(entry{true, {}, m_impl->scratch.str()});
}

void report::add_value(std::string_view name, bool skip_if_same)
{
    std::string text = m_impl->scratch.str();
    if (skip_if_same && (text == name || is_literal_operand(name)))
        return;
    m_impl->lines.push_back(entry{false, std::string(name), std::move(text)});
}

void print_formatted(std::ostream& out, fmt::format_args args)
{
#if defined(__cpp_exceptions) || defined(_CPPUNWIND)
    // A formatter that throws must not hide the verify failure
    try
    {
        out << fmt::vformat("{}", args);
    }
    catch (const std::exception& e)
    {
        out << "<fmt error: " << e.what() << ">";
    }
#else
    out << fmt::vformat("{}", args);
#endif
}

#if defined(__GNUC__) || defined(__clang__)
// Not inlined, so the return address is the cold failure function
__attribute__((noinline))
#endif
void report::fail() noexcept
{
    const impl& r = *m_impl;
    std::string out;
    out += r.file;
    out += ':';
    out += std::to_string(r.line);
    out += ": ";
    out += r.macro;
    out += " failed in ";
    out += r.function;
    out += "(): ";
    out += r.expression;
    out += '\n';

    // Align the '=' of the values, within reason
    std::size_t width = 0;
    for (const auto& l : r.lines)
        if (!l.is_message)
            width = std::max(width, l.name.size());
    width = std::min<std::size_t>(width, 32);

    // Messages first (what went wrong), then the values (the evidence)
    for (const auto& l : r.lines)
    {
        if (!l.is_message)
            continue;
        out += "  ";
        out += l.text;
        out += '\n';
    }
    for (const auto& l : r.lines)
    {
        if (l.is_message)
            continue;
        out += "  ";
        out += l.name;
        if (l.name.size() < width)
            out.append(width - l.name.size(), ' ');
        out += " = ";
        out += l.text;
        out += '\n';
    }

    std::fflush(stdout);
    std::fputs(out.c_str(), stderr);
    std::fflush(stderr);
#if defined(VERIFY_HAS_STACK_TRACE)
    print_stack_trace(__builtin_return_address(0));
#endif
    std::abort();
}
}
}
