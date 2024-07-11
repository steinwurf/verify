#ifndef STEINWURF_VERIFY_BACKENDS_STUB_HPP
#define STEINWURF_VERIFY_BACKENDS_STUB_HPP

// ----------------------------------------------------
// Variadic Function Helper Macro
// ----------------------------------------------------
// Inspired by: https://stackoverflow.com/a/26408195
#define __VERIFY_NARG__(...)  __VERIFY_NARG_I_(__VA_ARGS__,__VERIFY_RSEQ_N())
#define __VERIFY_NARG_I_(...) __VERIFY_ARG_N(__VA_ARGS__)
#define __VERIFY_ARG_N( \
      _1, _2, _3, _4, _5, _6, _7, _8, _9,_10, \
     _11,_12,_13,_14,_15,_16,_17,_18,_19,_20, \
     _21,_22,_23,_24,_25,_26,_27,_28,_29,_30, \
     _31,_32,_33,_34,_35,_36,_37,_38,_39,_40, \
     _41,_42,_43,_44,_45,_46,_47,_48,_49,_50, \
     _51,_52,_53,_54,_55,_56,_57,_58,_59,_60, \
     _61,_62,_63,N,...) N
#define __VERIFY_RSEQ_N() \
     63,62,61,60,                   \
     59,58,57,56,55,54,53,52,51,50, \
     49,48,47,46,45,44,43,42,41,40, \
     39,38,37,36,35,34,33,32,31,30, \
     29,28,27,26,25,24,23,22,21,20, \
     19,18,17,16,15,14,13,12,11,10, \
     9,8,7,6,5,4,3,2,1,0
#define _VERIFY_VFUNC_(name, n) name##n
#define _VERIFY_VFUNC(name, n) _VERIFY_VFUNC_(name, n)
#define VERIFY_VFUNC(func, ...) _VERIFY_VFUNC(func, __VERIFY_NARG__(__VA_ARGS__)) (__VA_ARGS__)

// ----------------------------------------------------
// Begin Code
// ----------------------------------------------------

#define VERIFY_ASSERT(...) VERIFY_VFUNC(VERIFY_ASSERT, __VA_ARGS__)
#define VERIFY_ASSERT_DEBUG(...) ((void)0)

#define VERIFY_ASSERT1(expr) ((void)(expr))

// Brute force N-args with variadic macros.
// TODO: Is there a better way to do this?
#define VERIFY_ASSERT3(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT4(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT5(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT6(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT7(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT8(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT9(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT10(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT11(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT12(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT13(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT14(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT15(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT16(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT17(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT18(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT19(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT20(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT21(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT22(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT23(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT24(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT25(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT26(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT27(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT28(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT29(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT30(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT31(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT32(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT33(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT34(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT35(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT36(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT37(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT38(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT39(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT40(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT41(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT42(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT43(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT44(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT45(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT46(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT47(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT48(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT49(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT50(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT51(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT52(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT53(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT54(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT55(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT56(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT57(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT58(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT59(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT60(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT61(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT62(expr, ...) VERIFY_ASSERT1(expr)
#define VERIFY_ASSERT63(expr, ...) VERIFY_ASSERT1(expr)

#endif // STEINWURF_VERIFY_BACKENDS_STUB_HPP
