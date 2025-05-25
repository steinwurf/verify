// Copyright (c) Steinwurf ApS 2023.
// This file is licensed under the terms of the accompanying LICENSE.rst file.

#pragma once

#if __has_include(<valgrind/valgrind.h>)
#include <valgrind/valgrind.h>

inline auto is_running_valgrind() -> bool
{
    return RUNNING_ON_VALGRIND;
}
#else
inline auto is_running_valgrind() -> bool
{
    return false; // Valgrind not detected
}
#endif