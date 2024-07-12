#ifndef STEINWURF_VERIFY_HPP
#define STEINWURF_VERIFY_HPP

#ifndef NLIBASSERT
    #include "backends/libassert.hpp"
#else
    #ifndef NDEBUG
        #include "backends/assert.hpp"
    #else
        #include "backends/stub.hpp"
    #endif // NDEBUG
#endif // NLIBASSERT

#define VERIFY(...) VERIFY_IMPL(__VA_ARGS__)
#define VERIFY_DEBUG(...) VERIFY_DEBUG_IMPL(__VA_ARGS__)

#endif // STEINWURF_VERIFY_HPP
