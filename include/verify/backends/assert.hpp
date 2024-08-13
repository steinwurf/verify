#ifndef STEINWURF_VERIFY_BACKENDS_ASSERT_HPP
#define STEINWURF_VERIFY_BACKENDS_ASSERT_HPP

#include "../verify_variadic_macro.hpp"
#include <cassert>

#define VERIFY_IMPL(...) VERIFY_VFUNC(VERIFY_IMPL, __VA_ARGS__)

#ifndef NDEBUG
    #define VERIFY_DEBUG_IMPL(...) VERIFY_VFUNC(VERIFY_IMPL, __VA_ARGS__)
#else
    #define VERIFY_DEBUG_IMPL(...) ((void)0)
#endif // NDEBUG

#define VERIFY_IMPL1(expr) ((void)(expr)); assert((expr))
#define VERIFY_IMPL2(expr, err) ((void)(expr)); assert((expr) && err)

// Brute force N-args with variadic macros.
// TODO: Is there a better way to do this?
#define VERIFY_IMPL3(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL4(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL5(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL6(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL7(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL8(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL9(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL10(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL11(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL12(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL13(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL14(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL15(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL16(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL17(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL18(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL19(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL20(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL21(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL22(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL23(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL24(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL25(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL26(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL27(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL28(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL29(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL30(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL31(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL32(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL33(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL34(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL35(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL36(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL37(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL38(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL39(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL40(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL41(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL42(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL43(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL44(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL45(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL46(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL47(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL48(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL49(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL50(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL51(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL52(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL53(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL54(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL55(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL56(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL57(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL58(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL59(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL60(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL61(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL62(expr, err, ...) VERIFY_IMPL2(expr, err)
#define VERIFY_IMPL63(expr, err, ...) VERIFY_IMPL2(expr, err)

#endif // STEINWURF_VERIFY_BACKENDS_ASSERT_HPP