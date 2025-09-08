News for verify
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the git log.

Latest
------
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
