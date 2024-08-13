#ifndef STEINWURF_VERIFY_HPP
#define STEINWURF_VERIFY_HPP

#ifdef NDEBUG
    #include "verify/backends/stub.hpp"
#else
    #ifdef STEINWURF_VERIFY_USE_LIBASSERT
        #include "verify/backends/libassert.hpp"
    #else
        #include "verify/backends/assert.hpp"
    #endif // STEINWURF_VERIFY_USE_LIBASSERT
#endif // NDEBUG

#define VERIFY(...) VERIFY_IMPL(__VA_ARGS__)
#define VERIFY_DEBUG(...) VERIFY_DEBUG_IMPL(__VA_ARGS__)

#endif // STEINWURF_VERIFY_HPP
