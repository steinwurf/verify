// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// The native backend, the default.
#undef NDEBUG
#undef STEINWURF_VERIFY_USE_CASSERT

#define VERIFY_BENCH_NS native_backend
#define VERIFY_BENCH_BACKEND_NAME "native"
#define VERIFY_BENCH_REGISTER register_native
#include "scenarios.hpp"
