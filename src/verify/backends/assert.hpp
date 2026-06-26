// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cassert>

#include "../verify_variadic_overload_macro.hpp"

#define VERIFY_IMPL1(condition) assert(condition)

// Preserve the optional message in the expression printed by assert without
// evaluating it. Extra arguments are ignored by the non-libassert backend.
#define VERIFY_IMPL2(condition, message) assert((condition) && #message)

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
