// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#ifdef NDEBUG
#define VERIFY(...) ((void)0)
#else
// Select the appropriate backend based on the C++ standard and configuration.
#ifdef STEINWURF_VERIFY_USE_LIBASSERT
#include "backends/libassert.hpp"
#else
#include "backends/assert.hpp"
#endif  // STEINWURF_VERIFY_USE_LIBASSERT

#ifdef _MSC_VER
#define VERIFY VERIFY_IMPL
#else
#define VERIFY(...) VERIFY_IMPL(__VA_ARGS__)
#endif  // _MSC_VER
#endif  // NDEBUG
