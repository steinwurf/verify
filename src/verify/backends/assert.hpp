// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstdlib>
#include <iostream>

#include "../verify_variadic_overload_macro.hpp"

#if defined(_WIN32)
#include <windows.h>
#elif defined(__APPLE__)
#include <mach-o/dyld.h>
#elif defined(__linux__)
#include <libgen.h>
#include <limits.h>
#include <string.h>
#include <unistd.h>
#endif

// Non-constexpr runtime version (platform-specific)
inline const char* binary_name_runtime()
{
#if defined(_WIN32)
    static char exe_path[MAX_PATH] = {0};
    if (exe_path[0] == 0)
    {
        GetModuleFileNameA(NULL, exe_path, MAX_PATH);
        // Optionally extract basename here
    }
    return exe_path;
#elif defined(__APPLE__)
    static char exe_path[1024] = {0};
    uint32_t size = sizeof(exe_path);
    if (exe_path[0] == 0 && _NSGetExecutablePath(exe_path, &size) == 0)
    {
        // Optionally extract basename here
    }
    return exe_path;
#elif defined(__linux__)
    static char exe_path[PATH_MAX];
    ssize_t len = readlink("/proc/self/exe", exe_path, sizeof(exe_path) - 1);
    if (len != -1)
    {
        exe_path[len] = '\0';
    }
    return basename(exe_path);
#else
    return "";
#endif
}

#if defined(__clang__) || defined(__GNUC__)
#define VERIFY_FUNCTION __PRETTY_FUNCTION__
#else
#define VERIFY_FUNCTION __FUNCTION__
#endif
#define VERIFY_IMPL1(condition)                                           \
    do                                                                    \
    {                                                                     \
        if (!(condition))                                                 \
        {                                                                 \
            std::cerr << binary_name_runtime() << ": " << __FILE__ << ":" \
                      << __LINE__ << ": " << VERIFY_FUNCTION              \
                      << ": Assertion `" << #condition << "' failed."     \
                      << std::endl;                                       \
            std::abort();                                                 \
        }                                                                 \
    } while (false)

#define VERIFY_IMPL2(condition, message)                                  \
    do                                                                    \
    {                                                                     \
        if (!(condition))                                                 \
        {                                                                 \
            std::cerr << binary_name_runtime() << ": " << __FILE__ << ":" \
                      << __LINE__ << ": " << VERIFY_FUNCTION              \
                      << ": Assertion `" << #condition << "' failed, "    \
                      << #message << "." << std::endl;                    \
            std::abort();                                                 \
        }                                                                 \
    } while (false)

// Brute force N-args with variadic macros.
// TODO: Is there a better way to do this?
#define VERIFY_IMPL3(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL4(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL5(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL6(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL7(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL8(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL9(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL10(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL11(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL12(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL13(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL14(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL15(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL16(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL17(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL18(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL19(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL20(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL21(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL22(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL23(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL24(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL25(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL26(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL27(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL28(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL29(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL30(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL31(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL32(expr, err, ...) VERIFY_IMPL2(expr, err)

#define VERIFY_IMPL(...) __VERIFY_OVERLOAD(VERIFY_IMPL, __VA_ARGS__)
