#ifndef STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
#define STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP

#include <libassert/assert.hpp>

#define VERIFY_ASSERT(expr, ...) LIBASSERT_INVOKE(expr, "ASSERT", assertion, , __VA_ARGS__)
#define VERIFY_ASSERT_DEBUG(expr, ...) LIBASSERT_INVOKE(expr, "DEBUG_ASSERT", assertion, , __VA_ARGS__)

#endif // STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
