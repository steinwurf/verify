Verify
======

Verify is a meta assertion library using ``libassert`` with a builtin fallback
to ``cassert`` when required for unsupported platforms.


Usage
-----

There's three primary ways to call Verify:

1. ``VERIFY(condition)``
2. ``VERIFY(condition, error_string)``
3. ``VERIFY(condition, error_string, variables...)``

These do respectively:

* ``condition``: holds the boolean test. If this returns false it will trigger
  an assertion. This will always get evaluated regardless of build type.
* ``error_string``: can hold an extra human readable error message to explain
  what occurred.
* ``variables``: can hold a list of extra variables and will print their
  currently held data when the backend supports it (like libassert).

There's an alternative `VERIFY_DEBUG` macro with the same structure, which
notably will not get evaluated at all in `NDEBUG` builds.


Functionality
-------------

* When ``NDEBUG`` is not defined, both ``VERIFY`` and ``VERIFY_DEBUG`` will raise
  assertions.
* When ``NDEBUG`` is defined, ``VERIFY`` will evaluate the statement, but will not
  raise on its result. ``VERIFY_DEBUG`` will not evaluate the assertion at all.


Limitations
-----------

Due to how the variadic macros are defined header only, you may only have up to
32 arguments in a single call. If you have a usecase where you need more than
this, please open an issue.
