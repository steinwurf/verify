Verify
======

Verify is a meta assertion library using ``libassert`` with a builtin fallback
to a generic ``std::abort`` method when required for unsupported platforms.


Usage
-----

There's three primary ways to call Verify:

1. ``VERIFY(condition)``
2. ``VERIFY(condition, error_string)``
3. ``VERIFY(condition, error_string, variables...)``

These arguments do respectively:

* ``condition``: holds the boolean test. If this returns false it will trigger
  an assertion. This will always get evaluated unless ``NDEBUG`` is defined, then
  it would get substituted with a ``(void)0`` instead.
* ``error_string``: can hold an extra human readable error message to explain
  what occurred. Needs to support the ``<<`` to std:err. Recommended it to be a
  string literal.
* ``variables``: can hold a list of extra variables and will print their
  currently held data. Currently supported in ``libassert``, otherwise these
  arguments are ignored.

Limitations
-----------

Due to how the variadic macros are defined header only, you may only have up to
32 arguments in a single call. If you have a usecase where you need more than
this, please open an issue.
