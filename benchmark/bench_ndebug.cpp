// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// Baseline: VERIFY compiled out. This is the lower bound for any backend.
// NDEBUG must be defined before any standard header is included.
#ifndef NDEBUG
#define NDEBUG
#endif
#undef STEINWURF_VERIFY_USE_CASSERT

#define VERIFY_BENCH_NS ndebug
#define VERIFY_BENCH_BACKEND_NAME "ndebug"
#define VERIFY_BENCH_REGISTER register_ndebug
#include "scenarios.hpp"
