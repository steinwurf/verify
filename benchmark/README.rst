verify benchmark
================

Measures the cost of a *passing* ``VERIFY`` per backend. The same scenarios
(``scenarios.hpp``) are compiled once per backend, one translation unit each,
and linked into a single ``verify_benchmark`` binary:

* ``ndebug`` - ``VERIFY`` compiled out, the absolute floor.
* ``cassert`` - plain ``assert()``. The realistic floor for a backend that
  checks: a check that can abort mid-loop blocks vectorization no matter how
  cheap its failure path is.
* ``native`` - the native backend, the default. Should match ``cassert``.

The libassert backend, removed in verify 7, was 1.4-2.4x slower than
``cassert`` on a desktop and up to 23x slower on a Raspberry Pi 5.

Adding a backend means adding a ``bench_<name>.cpp`` next to the others and
registering it in ``registry.hpp`` / ``main.cpp``.

Desktop
-------

Benchmark the ``WurfRelease`` build type (``-O2 -g`` with verifies enabled),
which is what ships::

    python3 waf configure --cmake_build_type=WurfRelease
    python3 waf build
    ./build/verify_benchmark

Options: ``--elements=N`` (default 4096), ``--min-time-ms=N`` (50),
``--reps=N`` (7), ``--filter=SUBSTRING`` (matches scenario or backend) and
``--csv``.

Tiny loops are sensitive to code alignment, so differences of ~20% between two
builds of otherwise identical code can be noise. Confirm large differences in
the generated code::

    gdb -batch -ex "disassemble 'verify_bench::native_backend::parse_records'" build/verify_benchmark

Hardware counters for one backend at a time (``--filter`` also matches the
backend name)::

    perf stat -e instructions,cycles,branches ./build/verify_benchmark --filter=native

Remote (Raspberry Pi or any aarch64 Linux)
------------------------------------------

::

    benchmark/run_remote.sh user@host [verify_benchmark options...]

This cross-compiles with the zig aarch64 toolchain from ``cmake-toolchains``
(clang), builds a second binary on the remote with its own ``g++``, copies
everything to ``~/verify-bench`` and runs both. The remote needs ``ssh``,
``tar`` and optionally ``g++``; its login shell does not matter. ``perf`` works
on the Pi too: ``ssh user@host 'cd verify-bench && perf stat ./verify_benchmark_gcc'``.
