Verify
======

Verify is an assertion library. A passing ``VERIFY`` costs the same as a plain
``assert()``; a failing one prints the condition, an optional message and the
values you list, then aborts.

Verify depends on `fmt <https://github.com/steinwurf/fmt>`_ (the
``steinwurf::fmt`` target, linked publicly), which it uses to print types that
have an ``fmt::formatter``. Defining ``STEINWURF_VERIFY_USE_CASSERT`` makes
``VERIFY`` a plain ``assert()`` that ignores the extra arguments.


Usage
-----

VERIFY
~~~~~~

::

    VERIFY(condition);
    VERIFY(condition, "message");
    VERIFY(condition, "message", values...);
    VERIFY(condition, values...);

* ``condition``: anything convertible to ``bool``. If it is false the
  verify fails.
* string literals among the extra arguments are printed as messages.
* every other extra argument is printed as ``name = value``.

The extra arguments are only evaluated when the verify fails, so they cost
nothing while it passes. ``VERIFY`` is an expression, like ``assert``, and can
be used in ``constexpr`` functions.

VERIFY_EQ, VERIFY_NE, VERIFY_LT, VERIFY_LE, VERIFY_GT, VERIFY_GE
~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~

::

    VERIFY_EQ(lhs, rhs);
    VERIFY_LT(index, size, "index out of range", other_values...);

Like ``VERIFY(lhs == rhs, ...)``, but both operands are printed on failure. Each
operand is evaluated exactly once. These macros are statements, not
expressions. Example output::

    decoder.cpp:42: VERIFY_EQ failed in decode(): symbols.size() == 16u
      unexpected generation size
      symbols.size() = 3

Operands written as literals (``16u`` above) are not repeated.

Printing values
~~~~~~~~~~~~~~~

Values are printed with these rules:

* arithmetic types, ``bool`` (``true``/``false``), characters (``'a'``),
  ``int8_t``/``uint8_t`` as numbers, floating point at full precision
* strings (``"quoted"``), pointers (``0x...`` or ``nullptr``), enums (their
  underlying value)
* ``std::pair``, ``std::tuple`` and ``std::optional``
* containers: anything with ``begin()``/``end()`` as ``[a, b, c]``, maps as
  ``{key: value}``, at most 32 elements
* any type with an ``operator<<(std::ostream&, const T&)``
* any other type with an ``fmt::formatter`` (or ``format_as``)

When a type has both, ``operator<<`` is used. Containers, tuples and
``std::optional`` always use the format above, even where ``fmt/ranges.h`` or
``fmt/std.h`` gives them a formatter, so a type prints the same way in every
file. Their elements use the same rules, so a ``std::vector`` of a type with
an ``fmt::formatter`` prints each element with it.

A type that cannot be printed is a **compile error**, so a verify that never
fails in your tests still cannot fail to print. To make a type printable
without an ``operator<<`` or ``fmt::formatter`` (for example a third-party
type), or to print it differently in verify output, specialize
``verify::printer``::

    template <>
    struct verify::printer<my::type>
    {
        static void print(std::ostream& out, const my::type& value)
        {
            out << "my::type{" << value.x << "}";
        }
    };

``verify::print(out, value)`` prints a value with the same rules, which is
useful for members inside a ``printer``.

Stack trace
~~~~~~~~~~~

On Linux and macOS a failed verify also prints the call stack, starting at the
function with the failing verify. It is collected only after the check has
failed, so it costs nothing while verifies pass::

    st.cpp:5: VERIFY_LT failed in check(): i < v.size()
      index out of range
      i        = 3
      v.size() = 3
    stack trace:
      #0  ./app(+0x26bd)
      #1  ./app(+0x2e29) main
      #2  /lib/x86_64-linux-gnu/libc.so.6(+0x2a574)
      ...
    file and line (needs debug info):
      addr2line -f -C -i -e ./app 0x26bd 0x2e29
      addr2line -f -C -i -e /lib/x86_64-linux-gnu/libc.so.6 0x2a574 ...

Function names are shown for functions in the dynamic symbol table: shared
libraries, or an executable linked with ``-rdynamic`` (CMake:
``ENABLE_EXPORTS``). On Linux the trace ends with an ``addr2line`` command per
module. Run it on a build with debug info (``-g``) to get the function, file
and line of each frame, including inlined calls. verify does not run it
itself: a failing check never starts another process. ``llvm-symbolizer``
accepts the same module and offsets.

The trace needs glibc's or macOS's ``backtrace()``; on musl and Windows it is
left out.

Disabled verifies
~~~~~~~~~~~~~~~~~

With ``NDEBUG`` defined, ``VERIFY`` and the comparison macros evaluate nothing
and generate no code, but they are still compiled. Conditions must still be
valid, values must still be printable, and variables that are only used in
verifies do not trigger unused-variable warnings. The same holds for
``VERIFY_DEBUG`` when it is disabled.

VERIFY_DEBUG
~~~~~~~~~~~~

``VERIFY_DEBUG`` has the same syntax and usage as ``VERIFY``, but is specifically
targeted for debug builds.
When ``WURFDEBUG`` is defined, ``VERIFY_DEBUG`` functions identically to ``VERIFY``
and will trigger assertions and output messages as expected.
In all other cases, such as when building in release mode, ``VERIFY_DEBUG`` is
disabled as described above.

Limitations
-----------

Due to how the variadic macros are defined header only, you may only have up to
32 arguments in a single call. If you have a usecase where you need more than
this, please open an issue.
