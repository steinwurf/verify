#ifndef STEINWURF_VERIFY_HPP
#define STEINWURF_VERIFY_HPP

#ifndef NLIBASSERT
    #include "verify_backends/libassert.hpp"
#else
    #ifndef NDEBUG
        #include "verify_backends/assert.hpp"
    #else
        #include "verify_backends/stub.hpp"
    #endif // NDEBUG
#endif // NLIBASSERT

#define VERIFY(...) VERIFY_ASSERT(__VA_ARGS__)
#define VERIFY_DEBUG(...) VERIFY_ASSERT_DEBUG(__VA_ARGS__)

#endif // STEINWURF_VERIFY_HPP
