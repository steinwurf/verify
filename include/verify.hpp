#ifndef STEINWURF_VERIFY_HPP
#define STEINWURF_VERIFY_HPP

#ifdef NLIBASSERT
    #include "backends/assert.hpp"
#elif __cplusplus > 201402L
    #include "backends/libassert.hpp"
#else
    #include "backends/assert.hpp"
#endif

#define VERIFY(...) VERIFY_IMPL(__VA_ARGS__)
#define VERIFY_DEBUG(...) VERIFY_DEBUG_IMPL(__VA_ARGS__)

#endif // STEINWURF_VERIFY_HPP
