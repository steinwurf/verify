# Work in progress: native backend (delete this file before merging)

Context for picking this branch up on another machine. The README, NEWS and
code comments describe the result; this file covers why, what was decided and
what is left.

## Why

`VERIFY` with the libassert backend was expensive. libassert captures operands
by reference, which takes the address of locals and members: they are forced
to the stack and GCC adds a stack protector to the hot path. Measured with
`benchmark/`, a passing libassert `VERIFY` was 1.4-2.4x slower than `assert()`
on x86 and 1.7x-23x slower on a Raspberry Pi 5.

## What this branch does

- **Native backend, the default** (`src/verify/backends/native.hpp`,
  `src/verify/detail/fail.hpp`, `src/verify/verify.cpp`).
  - Hot path is `VERIFY_LIKELY(cond) ? (void)0 : fail(...)`. The failure
    function is `[[noreturn]]`, cold and noinline.
  - Small trivially copyable values are passed by value (`pass_t<T>`), so
    nothing has its address taken. Always-inline trampolines
    (`verify_failed`, `compare_failed`) deduce the types first.
  - `VERIFY` is an expression (works in constexpr functions and comma
    expressions). `VERIFY_EQ/NE/LT/LE/GT/GE` are `do {} while (false)`
    statements that evaluate each operand once and print both on failure.
  - Output: `file:line: MACRO failed in func(): expr`, then message lines,
    then aligned `name = value` lines, then a stack trace, then `abort()`.
    Literal operands (`16u`) are not repeated as `16u = 16`.
- **Printing** (`src/verify/print.hpp`): print order is `verify::printer<T>`
  specialization, then built-in rules (bool, chars, numbers, strings,
  pointers, arrays), then `operator<<`, then `fmt::formatter`, then enums,
  pair/tuple, optional and containers (max 32 elements, maps as `{k: v}`).
  Anything else is a `static_assert`.
  - `operator<<` wins over fmt, and containers/tuples/optional never use
    fmt, so a type prints the same way in every file whether or not
    `fmt/ranges.h` / `fmt/std.h` are included (otherwise ODR violation).
  - fmt formatting runs out of line in `verify.cpp` (`print_formatted`);
    the caller only builds `fmt::make_format_args`.
- **Disabled verifies** (NDEBUG, and `VERIFY_DEBUG` without `WURFDEBUG`) are
  a dead branch, `false ? unused(...) : 0`: nothing is evaluated, but the
  condition must compile and all values must be printable.
- **Stack trace** on failure via `backtrace()` + `dladdr()`
  (glibc/macOS; left out on musl and Windows). Printed as
  `module(+offset) [name]`, ready for `addr2line -f -C -i -e module offset`.
  Names only for symbols in the dynamic symbol table (`-rdynamic` /
  CMake `ENABLE_EXPORTS`). With GCC -O2 frame #0 is often unnamed because the
  failure branch lives in a local `.cold` clone; addr2line still resolves it.
  The target links `CMAKE_DL_LIBS` (old glibc needs `-ldl`).
- **Removed**: libassert and zlib dependencies, the libassert backend, the
  `no-libassert` workflow, the waf option `--no_libassert`, the CMake option
  `STEINWURF_LIBASSERT`, and the Clang flags `-Wno-c++20-extensions
  -Wno-gnu-zero-variadic-macro-arguments` that the target used to add to its
  users.
- **Added dependency**: `steinwurf/fmt` major 5 (fmt 11.0.2), linked
  publicly. The lock file was regenerated for fmt only; `toolchains` was kept
  at 1.0.8 on purpose (regenerating bumps it to 1.5.1).
- **cassert backend** still available with `STEINWURF_VERIFY_USE_CASSERT`.
- **Benchmark** in `benchmark/` (see `benchmark/README.rst`): one translation
  unit per backend (ndebug, cassert, native), linked into
  `verify_benchmark`. `benchmark/run_remote.sh user@host` cross-compiles with
  the zig aarch64 toolchain, builds again with g++ on the remote, and runs
  both. Works with a fish login shell (commands go through `bash -s`).

## Decisions made

- Baseline stays C++17 / GCC 8.3.
- Unprintable types are a compile error, with `verify::printer<T>` as the
  escape hatch. Standard containers are supported.
- fmt is a required dependency, not optional (optional per-file detection
  would make the same type print differently in different files).
- No backtrace library: `backtrace()`/`dladdr()` only.

## Results

- Pi 5 (Cortex-A76): native matches cassert within 1% with clang everywhere,
  and with GCC 14 everywhere except `checked_view_dot` (1.08x) and
  `checked_view_dot_cmp` (0.92x). Both confirmed as code layout (alignment
  nops, compare operand order) in the disassembly, not real cost.
- Desktop (GCC 15, x86): hot code identical to cassert apart from the
  operand order of one compare. Expect +-20-30% alignment noise in tiny
  loops; the Pi is much more stable.
- Stack trace and fmt support did not change either benchmark.
- Compile time per file including `verify.hpp` (g++ / clang++): 6.3.0 with
  libassert 0.20 / 0.23 s, native without fmt 0.15 / 0.19 s, native with fmt
  0.33 / 0.49 s. All of the fmt cost is the wrapper's `FMT_HEADER_ONLY`
  (`fmt/base.h` then includes `format.h`: 0.29 s vs 0.02 s compiled).
  Decided not to address now; the fix would be a compiled mode in
  `steinwurf/fmt`.

## Tested

24 tests pass with GCC WurfRelease, GCC Debug, Clang WurfRelease, Clang Debug,
ASan/UBSan and zig x86_64 musl. The library also builds with
`-fno-exceptions`.

## Not checked yet (CI will run these)

- clang-format (`clang-format --dry-run --Werror` in CI; not installed
  locally when this was written).
- MSVC (Windows CI): the native backend and fmt (`/utf-8` comes from the fmt
  wrapper) were never compiled with it.
- GCC 8.3 (Pi CI).

## Open question: release severity

The goal was a minor release, but these can break code that compiles with
6.3.0:

1. Disabled verifies are now compiled. In 6.3.0 `VERIFY` under NDEBUG and
   `VERIFY_DEBUG` without WURFDEBUG were `((void)0)`. Code that uses
   variables that only exist under `#ifndef NDEBUG` / `#ifdef WURFDEBUG`, or
   stale expressions, no longer builds in those configurations.
2. Unprintable extra arguments: libassert printed `<instance of T>`; now it
   is a `static_assert` (types with an `fmt::formatter` are fine again).
3. `waf configure --no_libassert` now fails; Clang users lose the two
   warning flags; anything that came in through libassert is gone.

Suggested next step: build the downstream projects against this branch, with
NDEBUG and without WURFDEBUG. If they all compile, a minor release is
defensible, with NEWS stating that disabled verifies are now compiled.

## Building

```
python3 waf configure --cmake_build_type=WurfRelease
python3 waf build
./build/verify_tests
./build/verify_benchmark
```
