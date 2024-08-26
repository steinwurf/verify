News for verify
===============

This file lists the major changes between versions. For a more detailed list of
every change, see the git log.

Latest
------
* tbd

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
