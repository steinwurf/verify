// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <libassert/assert.hpp>

#define VERIFY_IMPL(expr, ...) \
    LIBASSERT_INVOKE(expr, "VERIFY", assertion, , __VA_ARGS__)