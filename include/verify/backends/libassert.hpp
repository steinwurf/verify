#ifndef STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
#define STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP

#include "../verify_variadic_macro.hpp"
#include <libassert/assert.hpp>

#define VERIFY_IMPL(...)                                                       \
  VERIFY_VFUNC(VERIFY_IMPL, "VERIFY", assertion, __VA_ARGS__)

#ifndef NDEBUG
#define VERIFY_DEBUG_IMPL(...)                                                 \
  VERIFY_VFUNC(VERIFY_IMPL, "VERIFY_DEBUG", debug_assertion, __VA_ARGS__)
#else
#define VERIFY_DEBUG_IMPL(expr, ...) ((void)0)
#endif // NDEBUG

#define VERIFY_IMPL3(friendly_name, assert_kind, expr)                         \
  LIBASSERT_INVOKE(expr, friendly_name, assert_kind, , )
#define VERIFY_IMPL4(friendly_name, assert_kind, expr, ...)                    \
  LIBASSERT_INVOKE(expr, friendly_name, assert_kind, , __VA_ARGS__)

// Brute force N-args with variadic macros.
// TODO: Is there a better way to do this?
#define VERIFY_IMPL5(friendly_name, assert_kind, expr, ...)                    \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL6(friendly_name, assert_kind, expr, ...)                    \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL7(friendly_name, assert_kind, expr, ...)                    \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL8(friendly_name, assert_kind, expr, ...)                    \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL9(friendly_name, assert_kind, expr, ...)                    \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL10(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL11(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL12(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL13(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL14(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL15(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL16(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL17(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL18(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL19(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL20(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL21(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL22(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL23(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL24(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL25(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL26(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL27(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL28(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL29(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL30(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL31(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL32(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL33(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL34(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL35(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL36(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL37(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL38(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL39(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL40(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL41(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL42(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL43(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL44(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL45(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL46(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL47(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL48(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL49(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL50(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL51(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL52(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL53(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL54(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL55(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL56(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL57(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL58(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL59(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL60(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL61(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL62(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)
#define VERIFY_IMPL63(friendly_name, assert_kind, expr, ...)                   \
  VERIFY_IMPL4(friendly_name, assert_kind, expr, __VA_ARGS__)

#endif // STEINWURF_VERIFY_BACKENDS_LIBASSERT_HPP
