// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

#pragma once

#include <cstddef>
#include <cstdint>
#include <string>
#include <vector>

namespace verify_bench
{
// Input shared by all scenarios. Every value is chosen so that no VERIFY ever
// fails: we measure the cost of a passing check only.
struct data
{
    static constexpr std::uint32_t value_limit = 1u << 20;
    static constexpr std::size_t table_size = 256;
    // src symbols are below this, so `s < symbol_limit` is a real check
    static constexpr std::uint8_t symbol_limit = 128;

    std::vector<std::uint32_t> a;
    std::vector<std::uint32_t> b;
    std::vector<std::uint32_t> index; // random positions into a
    std::vector<std::uint8_t> src;
    std::vector<std::uint8_t> dst;
    std::vector<std::uint8_t> buffer;
    std::uint8_t table[table_size];

    // Number of elements each scenario processes per call
    std::size_t elements = 0;
};

using scenario_fn = std::uint64_t (*)(data&);

struct entry
{
    std::string scenario;
    std::string backend;
    scenario_fn fn;
};

struct registry
{
    std::vector<entry> entries;

    void add(const char* scenario, const char* backend, scenario_fn fn)
    {
        entries.push_back(entry{scenario, backend, fn});
    }
};

// One per backend translation unit
void register_ndebug(registry& r);
void register_cassert(registry& r);
void register_native(registry& r);
}
