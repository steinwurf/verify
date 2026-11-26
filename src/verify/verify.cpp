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
// File and line come from running addr2line (binutils) on the failure path
#if defined(__ELF__) && __has_include(<spawn.h>) && \
    __has_include(<sys/wait.h>) && __has_include(<fcntl.h>)
#define VERIFY_HAS_SYMBOLIZER 1
#include <cerrno>
#include <fcntl.h>
#include <map>
#include <spawn.h>
#include <sys/wait.h>
#include <unistd.h>
extern char** environ;
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

#if defined(VERIFY_HAS_SYMBOLIZER)
/// One source location of a frame. A frame has several when functions were
/// inlined into it: the innermost (where execution was) comes first.
struct source_location
{
    std::string function;
    std::string file_line;
};

/// Run `addr2line -a -f -C -i -e module address...` and return its output, or
/// an empty string if it could not be run or failed.
std::string run_addr2line(const char* module,
                          const std::vector<std::string>& addresses)
{
    int out[2];
    if (::pipe(out) != 0)
        return {};
    // Close-on-exec, so only the dup2'ed copy reaches the child
    ::fcntl(out[0], F_SETFD, FD_CLOEXEC);
    ::fcntl(out[1], F_SETFD, FD_CLOEXEC);

    posix_spawn_file_actions_t actions;
    ::posix_spawn_file_actions_init(&actions);
    ::posix_spawn_file_actions_addopen(&actions, 0, "/dev/null", O_RDONLY, 0);
    ::posix_spawn_file_actions_adddup2(&actions, out[1], 1);
    // Warnings about missing debug info would mix into the report
    ::posix_spawn_file_actions_addopen(&actions, 2, "/dev/null", O_WRONLY, 0);

    std::vector<char*> argv;
    for (const char* arg : {"addr2line", "-a", "-f", "-C", "-i", "-e"})
        argv.push_back(const_cast<char*>(arg));
    argv.push_back(const_cast<char*>(module));
    for (const auto& a : addresses)
        argv.push_back(const_cast<char*>(a.c_str()));
    argv.push_back(nullptr);

    // Without DEBUGINFOD_URLS: a failing verify must not download debug info
    std::vector<char*> env;
    for (char** e = environ; e != nullptr && *e != nullptr; ++e)
        if (std::string_view(*e).substr(0, 16) != "DEBUGINFOD_URLS=")
            env.push_back(*e);
    env.push_back(nullptr);

    pid_t pid = 0;
    int error = ::posix_spawnp(&pid, "addr2line", &actions, nullptr,
                               argv.data(), env.data());
    ::posix_spawn_file_actions_destroy(&actions);
    ::close(out[1]);
    if (error != 0)
    {
        ::close(out[0]);
        return {};
    }

    std::string result;
    char buffer[4096];
    for (;;)
    {
        ssize_t n = ::read(out[0], buffer, sizeof(buffer));
        if (n > 0)
            result.append(buffer, static_cast<std::size_t>(n));
        else if (n == 0 || errno != EINTR)
            break;
    }
    ::close(out[0]);

    int status = 0;
    while (::waitpid(pid, &status, 0) < 0 && errno == EINTR)
    {
    }
    if (!WIFEXITED(status) || WEXITSTATUS(status) != 0)
        return {};
    return result;
}

/// Parse `addr2line -a -f -i` output: per address a `0x...` line, then a
/// function and a file:line line for each (inlined) location.
std::vector<std::vector<source_location>>
parse_addr2line(const std::string& output)
{
    std::vector<std::vector<source_location>> frames;
    std::vector<std::string_view> lines;
    std::string_view rest(output);
    while (!rest.empty())
    {
        auto end = rest.find('\n');
        lines.push_back(rest.substr(0, end));
        rest = end == std::string_view::npos ? "" : rest.substr(end + 1);
    }
    for (std::size_t i = 0; i < lines.size(); ++i)
    {
        if (lines[i].substr(0, 2) == "0x")
        {
            frames.emplace_back();
            continue;
        }
        if (frames.empty() || i + 1 >= lines.size())
            break;
        std::string_view function = lines[i];
        std::string_view file_line = lines[++i];
        // "file:line (discriminator 2)"
        auto paren = file_line.find(" (");
        if (paren != std::string_view::npos)
            file_line = file_line.substr(0, paren);
        // Unknown: "??:0", "??:?" or "file:0"
        auto colon = file_line.rfind(':');
        if (file_line.substr(0, 2) == "??" || colon == std::string_view::npos ||
            file_line.substr(colon) == ":0" || file_line.substr(colon) == ":?")
            file_line = {};
        if (function == "??")
            function = {};
        if (function.empty() && file_line.empty())
            continue;
        // verify's own functions inlined at the failing VERIFY
        if (function.find("verify::detail::") != std::string_view::npos)
            continue;
        frames.back().push_back(
            source_location{std::string(function), std::string(file_line)});
    }
    return frames;
}

/// Look up file and line of each frame with addr2line, one run per module.
/// Frames it cannot resolve (no debug info, addr2line missing) stay empty.
std::vector<std::vector<source_location>>
symbolize(const std::vector<const char*>& modules,
          const std::vector<std::uintptr_t>& offsets)
{
    std::vector<std::vector<source_location>> result(modules.size());
    const char* setting = std::getenv("VERIFY_SYMBOLIZE");
    if (setting != nullptr && std::string_view(setting) == "0")
        return result;

    std::map<std::string, std::vector<std::size_t>> by_module;
    for (std::size_t i = 0; i < modules.size(); ++i)
        if (modules[i] != nullptr)
            by_module[modules[i]].push_back(i);

    for (const auto& [module, indices] : by_module)
    {
        std::vector<std::string> addresses;
        for (std::size_t i : indices)
        {
            char buffer[32];
            std::snprintf(buffer, sizeof(buffer), "0x%jx",
                          static_cast<std::uintmax_t>(offsets[i]));
            addresses.push_back(buffer);
        }
        auto frames = parse_addr2line(run_addr2line(module.c_str(), addresses));
        if (frames.size() != indices.size())
            continue;
        for (std::size_t k = 0; k < indices.size(); ++k)
            result[indices[k]] = std::move(frames[k]);
    }
    return result;
}
#endif

// Print the call stack, starting at the function that called the failing
// VERIFY. Only runs on the failure path, so it costs nothing while verifies
// pass. Each frame is printed as `module(+offset)`, which addr2line and
// llvm-symbolizer accept: `addr2line -f -C -e module offset`. On ELF systems
// addr2line is also run here, adding function names and file:line from the
// debug info when it is installed and the module has debug info. Without it,
// function names are only known for symbols in the dynamic symbol table
// (shared libraries, or executables linked with -rdynamic).
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

    // Module and module-relative offset of each frame
    std::vector<const char*> modules;
    std::vector<std::uintptr_t> offsets;
    std::vector<const char*> symbols;
    for (int i = first; i < count; ++i)
    {
        // A return address points after the call; step back into it so the
        // symbolized line is the call itself.
        auto address = reinterpret_cast<std::uintptr_t>(frames[i]) - 1;

        Dl_info info{};
        if (::dladdr(reinterpret_cast<void*>(address), &info) == 0 ||
            info.dli_fname == nullptr || info.dli_fname[0] == '\0')
        {
            modules.push_back(nullptr);
            offsets.push_back(address);
            symbols.push_back(nullptr);
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
        modules.push_back(info.dli_fname);
        offsets.push_back(offset);
        symbols.push_back(info.dli_sname);
    }

#if defined(VERIFY_HAS_SYMBOLIZER)
    auto locations = symbolize(modules, offsets);
#endif

    std::string out = "stack trace:\n";
    for (std::size_t i = 0; i < modules.size(); ++i)
    {
        char buffer[64];
        std::snprintf(buffer, sizeof(buffer), "  #%-2d ", static_cast<int>(i));
        out += buffer;

        if (modules[i] == nullptr)
        {
            std::snprintf(buffer, sizeof(buffer), "0x%jx\n",
                          static_cast<std::uintmax_t>(offsets[i]));
            out += buffer;
            continue;
        }

        std::snprintf(buffer, sizeof(buffer), "(+0x%jx)",
                      static_cast<std::uintmax_t>(offsets[i]));
        out += modules[i];
        out += buffer;

        // The name of the function the frame is in: from the debug info if
        // there is one (the outermost location), else the dynamic symbol
        std::string name;
#if defined(VERIFY_HAS_SYMBOLIZER)
        if (!locations[i].empty())
            name = locations[i].back().function;
#endif
        if (name.empty() && symbols[i] != nullptr)
            name = demangle(symbols[i]);
        if (!name.empty())
        {
            out += ' ';
            out += name;
        }
        out += '\n';

#if defined(VERIFY_HAS_SYMBOLIZER)
        // Where in that function, through any inlined calls (innermost first)
        const auto& chain = locations[i];
        for (std::size_t k = 0; k < chain.size(); ++k)
        {
            const bool inlined = k + 1 < chain.size();
            if (chain[k].file_line.empty() && !inlined)
                continue;
            out += "        at ";
            out += chain[k].file_line.empty() ? "??" : chain[k].file_line;
            if (inlined)
            {
                out += " in ";
                out += chain[k].function;
                out += " (inlined)";
            }
            out += '\n';
        }
#endif
    }
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
void report::fail()
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
