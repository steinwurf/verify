// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// Benchmark scenarios. This file is included once per backend translation unit
// (bench_*.cpp). Each includer configures NDEBUG / the backend macros, defines
// VERIFY_BENCH_NS and VERIFY_BENCH_REGISTER, and then includes this file, so the
// exact same code is compiled against every backend.

#ifndef VERIFY_BENCH_NS
#error "Define VERIFY_BENCH_NS before including scenarios.hpp"
#endif

#include <cstdint>
#include <cstring>
#include <vector>

#include <verify/verify.hpp>

#include "registry.hpp"

#if defined(__GNUC__) || defined(__clang__)
#define VERIFY_BENCH_NOINLINE __attribute__((noinline))
#elif defined(_MSC_VER)
#define VERIFY_BENCH_NOINLINE __declspec(noinline)
#else
#define VERIFY_BENCH_NOINLINE
#endif

namespace verify_bench
{
namespace VERIFY_BENCH_NS
{

// Indirect lookups through an index array. The bounds check cannot be proven
// away by the compiler (unlike `i < v.size()` in a loop bounded by v.size()),
// so this measures one real compare-and-branch per element.
VERIFY_BENCH_NOINLINE std::uint64_t gather_index(data& d)
{
    const std::vector<std::uint32_t>& v = d.a;
    const std::vector<std::uint32_t>& index = d.index;
    std::uint64_t sum = 0;
    for (std::size_t i = 0; i < index.size(); ++i)
    {
        std::uint32_t at = index[i];
        VERIFY(at < v.size());
        sum += v[at];
    }
    return sum;
}

// A small bounds-checked view, the typical "VERIFY in operator[]" pattern.
// The check lives in an inlined member function and compares against a
// member, so the backend must not force the members into memory.
struct checked_view
{
    const std::uint32_t* m_data;
    std::size_t m_size;

    std::uint32_t operator[](std::size_t i) const
    {
        VERIFY(i < m_size, "index out of bounds");
        return m_data[i];
    }

    std::size_t size() const
    {
        return m_size;
    }
};

VERIFY_BENCH_NOINLINE std::uint64_t checked_view_dot(data& d)
{
    checked_view a{d.a.data(), d.a.size()};
    checked_view b{d.b.data(), d.b.size()};
    VERIFY(a.size() == b.size(), "size mismatch");

    std::uint64_t sum = 0;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        sum += static_cast<std::uint64_t>(a[i]) * b[i];
    }
    return sum;
}

// Table lookup into an output buffer, checking each looked-up value. Similar
// to the inner loop of a finite field region operation.
VERIFY_BENCH_NOINLINE std::uint64_t region_table_xor(data& d)
{
    const std::vector<std::uint8_t>& src = d.src;
    std::vector<std::uint8_t>& dst = d.dst;
    const std::uint8_t* table = d.table;
    VERIFY(src.size() == dst.size(), "region size mismatch");

    for (std::size_t i = 0; i < src.size(); ++i)
    {
        std::uint8_t s = src[i];
        VERIFY(s < data::symbol_limit, "symbol out of range");
        dst[i] ^= table[s];
    }
    return dst[0] + dst[dst.size() - 1];
}

// A stateful reader that checks its own members on every read, the typical
// pattern of a parser or serializer.
class reader
{
public:
    reader(const std::uint8_t* data, std::size_t size) :
        m_data(data), m_size(size), m_offset(0)
    {
    }

    std::uint32_t read_u32()
    {
        VERIFY(m_offset + sizeof(std::uint32_t) <= m_size,
               "read past end of buffer");
        std::uint32_t value;
        std::memcpy(&value, m_data + m_offset, sizeof(value));
        m_offset += sizeof(value);
        return value;
    }

    std::size_t remaining() const
    {
        VERIFY(m_offset <= m_size);
        return m_size - m_offset;
    }

private:
    const std::uint8_t* m_data;
    std::size_t m_size;
    std::size_t m_offset;
};

VERIFY_BENCH_NOINLINE std::uint64_t parse_records(data& d)
{
    reader r(d.buffer.data(), d.buffer.size());
    std::uint64_t sum = 0;
    while (r.remaining() >= sizeof(std::uint32_t))
    {
        sum += r.read_u32();
    }
    return sum;
}

// A VERIFY carrying a message and extra values to print on failure. The extra
// arguments must cost nothing while the check passes.
VERIFY_BENCH_NOINLINE std::uint64_t verify_with_args(data& d)
{
    const std::vector<std::uint32_t>& v = d.a;
    std::uint64_t sum = 0;
    for (std::size_t i = 0; i < v.size(); ++i)
    {
        VERIFY(v[i] < data::value_limit, "value out of range", i, v[i],
               data::value_limit);
        sum += v[i];
    }
    return sum;
}


// The same reader using the comparison macros, which bind references to their
// operands (here members) and print both values on failure.
class reader_cmp
{
public:
    reader_cmp(const std::uint8_t* data, std::size_t size) :
        m_data(data), m_size(size), m_offset(0)
    {
    }

    std::uint32_t read_u32()
    {
        VERIFY_LE(m_offset + sizeof(std::uint32_t), m_size,
                  "read past end of buffer");
        std::uint32_t value;
        std::memcpy(&value, m_data + m_offset, sizeof(value));
        m_offset += sizeof(value);
        return value;
    }

    std::size_t remaining() const
    {
        VERIFY_LE(m_offset, m_size);
        return m_size - m_offset;
    }

private:
    const std::uint8_t* m_data;
    std::size_t m_size;
    std::size_t m_offset;
};

VERIFY_BENCH_NOINLINE std::uint64_t parse_records_cmp(data& d)
{
    reader_cmp r(d.buffer.data(), d.buffer.size());
    std::uint64_t sum = 0;
    while (r.remaining() >= sizeof(std::uint32_t))
    {
        sum += r.read_u32();
    }
    return sum;
}

// checked_view_dot with VERIFY_LT in operator[]
struct checked_view_cmp
{
    const std::uint32_t* m_data;
    std::size_t m_size;

    std::uint32_t operator[](std::size_t i) const
    {
        VERIFY_LT(i, m_size, "index out of bounds");
        return m_data[i];
    }

    std::size_t size() const
    {
        return m_size;
    }
};

VERIFY_BENCH_NOINLINE std::uint64_t checked_view_dot_cmp(data& d)
{
    checked_view_cmp a{d.a.data(), d.a.size()};
    checked_view_cmp b{d.b.data(), d.b.size()};
    VERIFY_EQ(a.size(), b.size(), "size mismatch");

    std::uint64_t sum = 0;
    for (std::size_t i = 0; i < a.size(); ++i)
    {
        sum += static_cast<std::uint64_t>(a[i]) * b[i];
    }
    return sum;
}
}

void VERIFY_BENCH_REGISTER(registry& r)
{
    const char* backend = VERIFY_BENCH_BACKEND_NAME;
    r.add("gather_index", backend, &VERIFY_BENCH_NS::gather_index);
    r.add("checked_view_dot", backend, &VERIFY_BENCH_NS::checked_view_dot);
    r.add("checked_view_dot_cmp", backend,
          &VERIFY_BENCH_NS::checked_view_dot_cmp);
    r.add("region_table_xor", backend, &VERIFY_BENCH_NS::region_table_xor);
    r.add("parse_records", backend, &VERIFY_BENCH_NS::parse_records);
    r.add("parse_records_cmp", backend, &VERIFY_BENCH_NS::parse_records_cmp);
    r.add("verify_with_args", backend, &VERIFY_BENCH_NS::verify_with_args);
}
}
