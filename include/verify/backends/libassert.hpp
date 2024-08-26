#ifndef STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
#define STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP

#include <libassert/assert.hpp>

#ifndef NDEBUG
#define VERIFY_IMPL(expr, ...)                                                 \
  LIBASSERT_INVOKE(expr, "VERIFY", assertion, , __VA_ARGS__)
#else
#define VERIFY_IMPL(...) ((void)0)
#endif // NDEBUG

#endif // STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
