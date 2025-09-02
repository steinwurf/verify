// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <libassert/assert.hpp>

#include "../verify_variadic_overload_macro.hpp"

#define VERIFY_IMPL1(expr) LIBASSERT_INVOKE(expr, "VERIFY", assertion, , )
#define VERIFY_IMPL2(expr, ...) \
    LIBASSERT_INVOKE(expr, "VERIFY", assertion, , __VA_ARGS__)

// Brute force N-args with variadic macros.
// TODO: Is there a better way to do this?
#define VERIFY_IMPL3(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL4(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL5(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL6(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL7(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL8(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL9(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL10(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL11(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL12(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL13(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL14(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL15(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL16(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL17(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL18(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL19(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL20(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL21(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL22(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL23(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL24(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL25(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL26(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL27(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL28(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL29(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL30(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL31(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL32(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)

#define VERIFY_IMPL(...) __VERIFY_OVERLOAD(VERIFY_IMPL, __VA_ARGS__)
