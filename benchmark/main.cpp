// Copyright (c) Steinwurf ApS 2016.
// All Rights Reserved
//
// Distributed under the "BSD License". See the accompanying LICENSE.rst file.

// Measures the cost of a *passing* VERIFY for each backend on the same code.
//
// Usage: verify_benchmark [--elements=N] [--min-time-ms=N] [--reps=N]
//                         [--filter=SUBSTRING] [--csv]
//
// Every scenario/backend pair is run --reps times. Each repetition calls the
// scenario enough times to take at least --min-time-ms and records the time per
// element. Backends of the same scenario are interleaved per repetition so
// drift (thermal throttling, frequency scaling) hits them all equally. The
// reported number is the median repetition. "vs ndebug" is relative to the
// scenario with VERIFY compiled out (the absolute floor). "vs cassert" is
// relative to a plain assert(), the realistic floor for any backend that
// actually checks: a check that can abort mid-loop blocks vectorization no
// matter how cheap the failure path is.

#include "registry.hpp"

#include <algorithm>
#include <chrono>
#include <cstdint>
#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <map>
#include <random>
#include <string>
#include <vector>

namespace
{
volatile std::uint64_t g_sink;

struct options
{
    std::size_t elements = 4096;
    double min_time_ms = 50;
    std::size_t reps = 7;
    std::string filter;
    bool csv = false;
};

bool parse_value(const char* arg, const char* name, std::string& out)
{
    std::size_t len = std::strlen(name);
    if (std::strncmp(arg, name, len) == 0 && arg[len] == '=')
    {
        out = arg + len + 1;
        return true;
    }
    return false;
}

options parse_options(int argc, char** argv)
{
    options o;
    for (int i = 1; i < argc; ++i)
    {
        std::string v;
        if (parse_value(argv[i], "--elements", v))
            o.elements = std::strtoull(v.c_str(), nullptr, 10);
        else if (parse_value(argv[i], "--min-time-ms", v))
            o.min_time_ms = std::strtod(v.c_str(), nullptr);
        else if (parse_value(argv[i], "--reps", v))
            o.reps = std::strtoull(v.c_str(), nullptr, 10);
        else if (parse_value(argv[i], "--filter", v))
            o.filter = v;
        else if (std::strcmp(argv[i], "--csv") == 0)
            o.csv = true;
        else
        {
            std::fprintf(stderr,
                         "usage: %s [--elements=N] [--min-time-ms=N] "
                         "[--reps=N] [--filter=SUBSTRING] [--csv]\n",
                         argv[0]);
            std::exit(2);
        }
    }
    if (o.elements < 8 || o.reps == 0)
    {
        std::fprintf(stderr, "--elements must be >= 8 and --reps >= 1\n");
        std::exit(2);
    }
    return o;
}

verify_bench::data make_data(std::size_t elements)
{
    using verify_bench::data;
    data d;
    d.elements = elements;
    std::mt19937 rng(42);
    std::uniform_int_distribution<std::uint32_t> value(0,
                                                       data::value_limit - 1);
    std::uniform_int_distribution<int> byte(0, 255);
    std::uniform_int_distribution<int> symbol(0, data::symbol_limit - 1);
    std::uniform_int_distribution<std::uint32_t> position(
        0, static_cast<std::uint32_t>(elements - 1));

    d.a.resize(elements);
    d.b.resize(elements);
    d.index.resize(elements);
    for (std::size_t i = 0; i < elements; ++i)
    {
        d.a[i] = value(rng);
        d.b[i] = value(rng);
        d.index[i] = position(rng);
    }
    d.src.resize(elements);
    d.dst.resize(elements);
    for (std::size_t i = 0; i < elements; ++i)
    {
        d.src[i] = static_cast<std::uint8_t>(symbol(rng));
        d.dst[i] = static_cast<std::uint8_t>(byte(rng));
    }
    for (std::size_t i = 0; i < data::table_size; ++i)
        d.table[i] = static_cast<std::uint8_t>(byte(rng));

    d.buffer.resize(elements * sizeof(std::uint32_t));
    for (auto& b : d.buffer)
        b = static_cast<std::uint8_t>(byte(rng));
    return d;
}

using clock_type = std::chrono::steady_clock;

// Returns nanoseconds per element for one repetition.
double run_once(verify_bench::scenario_fn fn, verify_bench::data& d,
                double min_time_ms)
{
    std::size_t calls = 1;
    for (;;)
    {
        auto start = clock_type::now();
        std::uint64_t acc = 0;
        for (std::size_t i = 0; i < calls; ++i)
            acc += fn(d);
        auto stop = clock_type::now();
        g_sink = acc;

        double ns =
            std::chrono::duration<double, std::nano>(stop - start).count();
        if (ns >= min_time_ms * 1e6)
            return ns / (static_cast<double>(calls) * d.elements);

        // Grow towards the target time, at most 10x per step
        double factor = ns > 0 ? (min_time_ms * 1e6 * 1.2) / ns : 10.0;
        factor = std::min(std::max(factor, 2.0), 10.0);
        calls = static_cast<std::size_t>(calls * factor) + 1;
    }
}

double median(std::vector<double> v)
{
    std::sort(v.begin(), v.end());
    std::size_t n = v.size();
    return n % 2 ? v[n / 2] : (v[n / 2 - 1] + v[n / 2]) / 2.0;
}
}

int main(int argc, char** argv)
{
    options opt = parse_options(argc, argv);

    verify_bench::registry reg;
    verify_bench::register_ndebug(reg);
    verify_bench::register_cassert(reg);
    verify_bench::register_native(reg);

    // Group by scenario, keeping registration order
    std::vector<std::string> scenarios;
    std::vector<std::string> backends;
    std::map<std::string, std::vector<const verify_bench::entry*>> by_scenario;
    for (const auto& e : reg.entries)
    {
        if (!opt.filter.empty() &&
            e.scenario.find(opt.filter) == std::string::npos &&
            e.backend.find(opt.filter) == std::string::npos)
            continue;
        if (!by_scenario.count(e.scenario))
            scenarios.push_back(e.scenario);
        if (std::find(backends.begin(), backends.end(), e.backend) ==
            backends.end())
            backends.push_back(e.backend);
        by_scenario[e.scenario].push_back(&e);
    }

    verify_bench::data d = make_data(opt.elements);

    if (opt.csv)
        std::printf("scenario,backend,ns_per_element_median,"
                    "ns_per_element_min,ratio_vs_ndebug,ratio_vs_cassert\n");
    else
    {
        std::printf("elements=%zu min-time-ms=%g reps=%zu\n\n", opt.elements,
                    opt.min_time_ms, opt.reps);
        std::printf("%-22s %-12s %14s %14s %10s %10s\n", "scenario", "backend",
                    "median ns/elem", "min ns/elem", "vs ndebug", "vs cassert");
    }

    for (const auto& scenario : scenarios)
    {
        const auto& entries = by_scenario[scenario];
        std::vector<std::vector<double>> samples(entries.size());

        // Warm up caches and branch predictors
        for (const auto* e : entries)
            run_once(e->fn, d, opt.min_time_ms / 5);

        for (std::size_t rep = 0; rep < opt.reps; ++rep)
            for (std::size_t i = 0; i < entries.size(); ++i)
                samples[i].push_back(
                    run_once(entries[i]->fn, d, opt.min_time_ms));

        auto median_of = [&](const char* backend)
        {
            for (std::size_t i = 0; i < entries.size(); ++i)
                if (entries[i]->backend == backend)
                    return median(samples[i]);
            return 0.0;
        };
        const double ndebug = median_of("ndebug");
        const double cassert = median_of("cassert");

        for (std::size_t i = 0; i < entries.size(); ++i)
        {
            double med = median(samples[i]);
            double min =
                *std::min_element(samples[i].begin(), samples[i].end());
            double vs_ndebug = ndebug > 0 ? med / ndebug : 0;
            double vs_cassert = cassert > 0 ? med / cassert : 0;
            if (opt.csv)
            {
                std::printf("%s,%s,%.4f,%.4f,%.3f,%.3f\n", scenario.c_str(),
                            entries[i]->backend.c_str(), med, min, vs_ndebug,
                            vs_cassert);
                continue;
            }
            char r1[16] = "-";
            char r2[16] = "-";
            if (vs_ndebug > 0)
                std::snprintf(r1, sizeof(r1), "%.2fx", vs_ndebug);
            if (vs_cassert > 0)
                std::snprintf(r2, sizeof(r2), "%.2fx", vs_cassert);
            std::printf("%-22s %-12s %14.4f %14.4f %10s %10s\n",
                        scenario.c_str(), entries[i]->backend.c_str(), med, min,
                        r1, r2);
        }
        if (!opt.csv)
            std::printf("\n");
        std::fflush(stdout);
    }
    return 0;
}
