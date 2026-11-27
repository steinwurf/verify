// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// The plain assert() backend: STEINWURF_VERIFY_USE_CASSERT.
#undef NDEBUG
#define STEINWURF_VERIFY_USE_CASSERT

#define VERIFY_BENCH_NS cassert_backend
#define VERIFY_BENCH_BACKEND_NAME "cassert"
#define VERIFY_BENCH_REGISTER register_cassert
#include "scenarios.hpp"
