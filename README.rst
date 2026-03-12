Verify
======

Verify is a meta assertion library using ``libassert`` with a builtin fallback
to a generic ``std::abort`` method when required for unsupported platforms.


Usage
-----

VERIFY
~~~~~~

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

VERIFY_DEBUG
~~~~~~~~~~~~

``VERIFY_DEBUG`` has the same syntax and usage as ``VERIFY``, but is specifically
targeted for debug builds.
When ``WURFDEBUG`` is defined, ``VERIFY_DEBUG`` functions identically to ``VERIFY``
and will trigger assertions and output messages as expected.
In all other cases, such as when building in release mode, ``VERIFY_DEBUG`` is
effectively disabled and compiles to a no-op.

Limitations
-----------

Due to how the variadic macros are defined header only, you may only have up to
32 arguments in a single call. If you have a usecase where you need more than
this, please open an issue.
