Verify
======

Verify is a meta assertion library using ``libassert`` with a graceful fallback
to ``cassert`` when on unsupported platforms.

Usage
-----

There's three primary ways to call Verify:

1. ``VERIFY(condition)``
2. ``VERIFY(condition, error_string)``
3. ``VERIFY(condition, error_string, variables...)``

These do respectively:

* ``condition``: holds the boolean test. If this returns false it will trigger an assertion in debug builds.
* ``error_string``: can hold an extra human readable error message to explain what occurred, and will get attached to the assertion statement.
* ``variables``: can hold a list of extra variables and their currently held data to be printed when the backend supports it.

There's also a `VERIFY_DEBUG` macro of the same structure. Which will not get evaluated in release builds (given `NDEBUG`).

Definitions
-----------

* ``NDEBUG``: Disables assertions from getting validated. ``VERIFY_DEBUG`` will further be substituted away entirely with ``(void)0``.
* ``NLIBASSERT``: Forces ``libassert`` to not be chosen as a backend.

Limitations
-----------

Due to how the variadic macros are defined header only, you may only have up to
63 arguments in a single call. If you have a usecase where you need more than
this, please open an issue.
