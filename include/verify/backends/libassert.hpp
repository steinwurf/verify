#ifndef STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
#define STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP

#include "../verify_variadic_overload_macro.hpp"
#include <libassert/assert.hpp>

// --- Begin VERIFY_IMPL ---

#define VERIFY_IMPL1(expr) LIBASSERT_INVOKE(expr, "VERIFY", assertion, , )
#define VERIFY_IMPL2(expr, ...)                                                \
  LIBASSERT_INVOKE(expr, "VERIFY", assertion, , __VA_ARGS__)

// Brute force N-args with variadic macros.
#define VERIFY_IMPL3(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL4(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL5(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL6(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL7(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL8(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL9(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL10(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL11(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL12(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL13(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL14(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL15(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL16(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL17(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL18(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL19(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL20(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL21(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL22(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL23(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL24(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL25(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL26(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL27(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL28(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL29(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL30(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL31(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)
#define VERIFY_IMPL32(expr, ...) VERIFY_IMPL2(expr, __VA_ARGS__)

#define VERIFY_IMPL(...) __VERIFY_OVERLOAD(VERIFY_IMPL, __VA_ARGS__)

// --- Begin VERIFY_DEBUG_IMPL ---

#ifndef NDEBUG
#define VERIFY_DEBUG_IMPL1(expr)                                               \
  LIBASSERT_INVOKE(expr, "VERIFY_DEBUG", debug_assertion, , )
#define VERIFY_DEBUG_IMPL2(expr, ...)                                          \
  LIBASSERT_INVOKE(expr, "VERIFY_DEBUG", debug_assertion, , __VA_ARGS__)

// Brute force N-args with variadic macros.
#define VERIFY_DEBUG_IMPL3(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL4(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL5(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL6(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL7(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL8(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL9(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL10(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL11(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL12(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL13(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL14(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL15(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL16(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL17(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL18(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL19(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL20(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL21(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL22(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL23(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL24(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL25(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL26(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL27(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL28(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL29(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL30(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL31(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)
#define VERIFY_DEBUG_IMPL32(expr, ...) VERIFY_DEBUG_IMPL2(expr, __VA_ARGS__)

#define VERIFY_DEBUG_IMPL(...) __VERIFY_OVERLOAD(VERIFY_DEBUG_IMPL, __VA_ARGS__)
#else
#define VERIFY_DEBUG_IMPL(expr, ...) ((void)0)
#endif // NDEBUG

#endif // STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
