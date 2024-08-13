#ifndef STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
#define STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP

#include <libassert/assert.hpp>

#define VERIFY_IMPL(expr, ...) LIBASSERT_INVOKE(expr, "VERIFY", assertion, , __VA_ARGS__)

#ifndef NDEBUG
    #define VERIFY_DEBUG_IMPL(expr, ...) LIBASSERT_INVOKE(expr, "VERIFY_DEBUG", debug_assertion, , __VA_ARGS__)
#else
    #define VERIFY_DEBUG_IMPL(expr, ...) ((void)0)
#endif // NDEBUG

#endif // STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
