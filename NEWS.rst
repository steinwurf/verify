News for verify
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the git log.

Latest
------
* tbd

7.0.0
-----
* Major: Remove the libassert backend and the libassert and zlib
  dependencies. A passing libassert ``VERIFY`` was 1.4-2.4x slower than
  ``assert()`` on x86 and up to 23x slower on a Raspberry Pi 5. The waf option
  ``--no_libassert`` and the CMake option ``STEINWURF_LIBASSERT`` are removed.
* Major: Add a native backend, now the default. A passing ``VERIFY`` costs the
  same as ``assert()``; a failing one prints the message and the values of the
  extra arguments. The cassert backend is still available with
  ``STEINWURF_VERIFY_USE_CASSERT``.
* Minor: Add the fmt dependency (``steinwurf::fmt``, linked publicly). Types
  with an ``fmt::formatter`` and no ``operator<<`` are printed with fmt.
* Minor: A failed verify prints a stack trace as module and offset, with
  function names where the dynamic symbol table has them. On Linux it ends
  with a ready-to-copy ``addr2line`` command per module for file and line.
  Uses ``backtrace()`` and ``dladdr()``: glibc and macOS only, no
  dependencies. The ``steinwurf::verify`` target links ``CMAKE_DL_LIBS``.
* Minor: A failing verify always aborts. An exception thrown while printing a
  value is shown in place of the value. ``char*`` and ``const char*`` values
  print at most 64 characters, as they may point at data without a NUL.
* Major: The ``steinwurf::verify`` target no longer adds
  ``-Wno-c++20-extensions`` and ``-Wno-gnu-zero-variadic-macro-arguments`` to
  its users with Clang.
* Major: Extra arguments to ``VERIFY`` must be printable, or compilation fails
  with a ``static_assert`` in builds where verifies are enabled. Specialize
  ``verify::printer<T>`` for types without an ``operator<<``.
* Minor: Add ``VERIFY_EQ``, ``VERIFY_NE``, ``VERIFY_LT``, ``VERIFY_LE``,
  ``VERIFY_GT`` and ``VERIFY_GE``, which print both operands on failure.
* Minor: Add ``verify::print`` and ``verify::printer``, with support for
  standard containers, pairs, tuples and optionals.
* Minor: Remove the GCC Oldest and Clang Oldest CI builds.
* Minor: Add ``verify_benchmark``, measuring the cost of a passing ``VERIFY``
  per backend, and ``benchmark/run_remote.sh`` to run it on an aarch64 host
  such as a Raspberry Pi.

6.3.0
-----
* Minor: Update libassert to v2.2.1, which builds cpptrace 1.0.4. cpptrace
  0.8.2 (used by libassert v2.1.5) called ``dwarf_dealloc`` on strings that
  point into the debug info, so printing the stack trace of a failed
  ``VERIFY`` read misaligned memory, which UBSan reports.

6.2.0
-----
* Minor: Use cassert as fallback as that is much faster than the current fallback

6.1.0
-----
* Minor: Add ``VERIFY_DEBUG`` macro. When ``WURFRELEASE`` is defined it is a no-op;
  when ``WURFDEBUG`` is defined it behaves like ``VERIFY``.

6.0.4
-----
* Patch: Move no-libassert github workflows to its own file.
* Patch: Changed assert.hpp to no longer use static variables and instead
  return a std::string. This fixes any possible issues with thread safety.

6.0.3
-----
* Patch: Fix macros so that libassert is now re-enabled when available.

6.0.2
-----
* Patch: Reverted to v5 regarding the include/verify because in the new version a warning triggered when using clang.

6.0.1
-----
* Patch: Remove -pedantic-errors from CMakeLists.txt.

6.0.0
-----
* Major: Only CMake build system is supported.
* Major: Simplified the use of libassert. The use of cassert also no longer
  support a message.

5.3.0
-----
* Patch: Fixed issue with clang 19.
* Minor: Added license headers to all files.

5.2.0
-----
* Minor: Switched to use https://github.com/steinwurf/libassert-source
  as libassert source repository.
* Minor: Update libassert to version 2.1.2

5.1.4
-----
* Patch: Handle pandantic errors in CMakeLists.txt

5.1.3
-----
* Patch: Forward ``-fPIC`` flag to cmake builds.

5.1.2
-----
* Patch: Fix intermittent linking issues on linux machines

5.1.1
-----
* Patch: Fix linking issues on raspberry pi machines

5.1.0
-----
* Minor: Set up transparent zlib building when system library is not found

5.0.1
-----
* Patch: Fix MacOS related build warnings and errors

5.0.0
-----
* Major: Change --force-libassert to --force-use-libassert in waf for clarity
* Patch: Add missing headers in assert.hpp backend.

4.0.0
-----
* Major: DEBUG_VERIFY is now replaced by VERIFY as both functions ended up doing the same thing.
* Minor: Waf builds on Windows fall back on a generic assert instead of using libassert.
* Minor: The error message in the generic assert is now more readable.

3.0.2
-----
* Patch: Add to tag-registry again

3.0.1
-----
* Patch: Add to tag-registry

3.0.0
-----
* Major: Moved ``verify.hpp`` to ``verify/verify.hpp`` for windows support.
* Major: Refactor of most build code and codebase to correctly support windows as a build target.

2.0.0
-----
* Major: Restructure include folder into verify subdirectory.
* Major: Rename toggle STEINWURF_VERIFY_NO_LIBASSERT to STEINWURF_VERIFY_USE_LIBASSERT for clarity.
* Minor: Correctly expose, link and import libassert package.

1.1.0
-----
* Minor: Update libassert wrapper to version 2 to avoid upstream conflicts when building.
* Minor: Fix incorrect function name being shown when calling VERIFY.

1.0.0
-----
* Major: Initial release
