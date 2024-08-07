#ifndef STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
#define STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP

#include <libassert/assert.hpp>

#define VERIFY_IMPL(expr, ...) LIBASSERT_INVOKE(expr, "VERIFY", assertion, , __VA_ARGS__)
#define VERIFY_DEBUG_IMPL(expr, ...) LIBASSERT_INVOKE(expr, "VERIFY_DEBUG", debug_assertion, , __VA_ARGS__)

#endif // STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
