#!/usr/bin/env bash
# Run verify_benchmark on a remote aarch64 Linux machine (e.g. a Raspberry Pi).
#
#   benchmark/run_remote.sh user@host [verify_benchmark args...]
#
# Produces two binaries and runs both on the remote:
#
#   verify_benchmark_clang  cross-compiled here with the zig aarch64 toolchain
#   verify_benchmark_gcc    compiled on the remote with its own g++ (if any)
#
# The remote only needs ssh, tar and (optionally) g++. Its login shell does not
# matter: every remote command is run through bash.

set -euo pipefail

if [[ $# -lt 1 ]]; then
    echo "usage: $0 user@host [verify_benchmark args...]" >&2
    exit 2
fi

host="$1"
shift
args=""
if [[ $# -gt 0 ]]; then
    args=$(printf ' %q' "$@")
fi

root="$(cd "$(dirname "${BASH_SOURCE[0]}")/.." && pwd)"
build="$root/build_aarch64"
remote_dir="verify-bench"
toolchain="$root/resolve_symlinks/toolchains/zig-toolchain-aarch64-linux-gnu.cmake"

if [[ ! -e "$toolchain" ]]; then
    echo "Missing $toolchain - run 'python3 waf configure' once to resolve" >&2
    exit 1
fi

# Run a script on the remote with bash, whatever the login shell is. The script
# goes over stdin so no quoting survives a foreign shell (e.g. fish).
remote() {
    printf '%s\n' "$1" | ssh "$host" bash -s
}

echo "### Cross-compiling for aarch64 with zig" >&2
if [[ ! -e "$build/CMakeCache.txt" ]]; then
    cmake -S "$root" -B "$build" \
        -DCMAKE_TOOLCHAIN_FILE="$toolchain" \
        -DCMAKE_BUILD_TYPE=WurfRelease \
        -DSTEINWURF_RESOLVE="$root/resolve_symlinks" >/dev/null
fi
# zig prints a lot of harmless "not a recognized feature" warnings, so the
# build output goes to a log and is only shown (filtered) on failure
if ! cmake --build "$build" --target verify_benchmark -j "$(nproc)" \
    >"$build/build.log" 2>&1; then
    grep -v "is not a recognized feature" "$build/build.log" | tail -40 >&2
    echo "Cross-compile failed, full log: $build/build.log" >&2
    exit 1
fi

echo "### Copying to $host:$remote_dir" >&2
remote "mkdir -p $remote_dir"
scp -q "$build/verify_benchmark" "$host:$remote_dir/verify_benchmark_clang"
sources="$build/verify_bench_sources.tar.gz"
# fmt headers (header-only, as the steinwurf::fmt target uses them) end up in
# include/ on the remote
tar czhf "$sources" -C "$root" src benchmark \
    -C "$root/resolve_symlinks/fmt-source" include
scp -q "$sources" "$host:$remote_dir/sources.tar.gz"
remote "cd $remote_dir && rm -rf src benchmark include && tar xzf sources.tar.gz"

echo "### Building natively on $host" >&2
remote "cd $remote_dir && if command -v g++ >/dev/null; then
    g++ -std=c++17 -O2 -g -UNDEBUG -DWURFRELEASE -Isrc -Iinclude -DFMT_HEADER_ONLY \
        benchmark/main.cpp benchmark/bench_ndebug.cpp \
        benchmark/bench_cassert.cpp benchmark/bench_native.cpp \
        src/verify/verify.cpp \
        -o verify_benchmark_gcc -ldl
else
    rm -f verify_benchmark_gcc
    echo 'no g++ on remote, skipping native build' >&2
fi"

remote "cd $remote_dir
echo \"### \$(cat /proc/device-tree/model 2>/dev/null | tr -d '\\0' || uname -m)\"
echo \"### clang (zig cross-compile)\"
./verify_benchmark_clang$args
if [ -x verify_benchmark_gcc ]; then
    echo \"### \$(g++ --version | head -1) (native)\"
    ./verify_benchmark_gcc$args
fi"
