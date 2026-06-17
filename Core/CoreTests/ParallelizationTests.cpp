//----------------------------------------------------------------------------------------------
// ParallelizationTests.cpp
//
// Unit tests for InfinityLearn's parallelization features that use oneTBB.
// These tests verify that oneTBB can be included and used, and that the InfinityLearn wrappers
// function as expected.
//----------------------------------------------------------------------------------------------

#include <atomic>                       // std::atomic.
#include <catch2/catch_test_macros.hpp> // TEST_CASE, REQUIRE.
#include <cstddef>                      // std::size_t.
#include <oneapi/tbb/blocked_range.h>   // oneapi::tbb::blocked_range.
#include <oneapi/tbb/info.h>            // oneapi::tbb::info.
#include <oneapi/tbb/parallel_for.h>    // oneapi::tbb::parallel_for.
#include <oneapi/tbb/parallel_reduce.h> // oneapi::tbb::parallel_reduce.
#include <vector>                       // std::vector.

namespace
{
constexpr std::size_t kNumValues = 10000;
}

using namespace oneapi::tbb;

//----------------------------------------------------------------------------------------------
// oneTBB smoke tests
//----------------------------------------------------------------------------------------------
TEST_CASE("oneTBB can query default concurrency", "[Core][Parallelization]")
{
    const int concurrency = info::default_concurrency();
    REQUIRE(concurrency > 0);
}

TEST_CASE("oneTBB parallel_for visits every index exactly once", "[Core][Parallelization]")
{
    std::vector<std::atomic<int>> visit_counts(kNumValues);

    parallel_for(blocked_range<std::size_t>(0, kNumValues),
                 [&](const blocked_range<std::size_t>& range)
                 {
                     for (std::size_t i = range.begin(); i < range.end(); ++i)
                     {
                         visit_counts[i].fetch_add(1, std::memory_order_relaxed);
                     }
                 });

    for (std::size_t i = 0; i < kNumValues; ++i)
    {
        REQUIRE(visit_counts[i].load(std::memory_order_relaxed) == 1);
    }
}

TEST_CASE("oneTBB parallel_reduce computes expected sum", "[Core][Parallelization]")
{
    const std::size_t sum = parallel_reduce(
        blocked_range<std::size_t>(0, kNumValues), std::size_t{0},
        [](const blocked_range<std::size_t>& range, std::size_t local_sum)
        {
            for (std::size_t i = range.begin(); i < range.end(); ++i)
            {
                local_sum += i;
            }

            return local_sum;
        },
        [](std::size_t a, std::size_t b) { return a + b; });

    const std::size_t expected = (kNumValues - 1) * kNumValues / 2;

    REQUIRE(sum == expected);
}

TEST_CASE("oneTBB parallel_for supports blocked ranges with grain size", "[Core][Parallelization]")
{
    constexpr std::size_t grain_size = 256;

    std::vector<std::atomic<int>> visit_counts(kNumValues);

    parallel_for(blocked_range<std::size_t>(0, kNumValues, grain_size),
                 [&](const blocked_range<std::size_t>& range)
                 {
                     REQUIRE(range.begin() < range.end());
                     REQUIRE(range.end() <= kNumValues);

                     for (std::size_t i = range.begin(); i < range.end(); ++i)
                     {
                         visit_counts[i].fetch_add(1, std::memory_order_relaxed);
                     }
                 });

    for (std::size_t i = 0; i < kNumValues; ++i)
    {
        REQUIRE(visit_counts[i].load(std::memory_order_relaxed) == 1);
    }
}
