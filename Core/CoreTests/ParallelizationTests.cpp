//----------------------------------------------------------------------------------------------
// ParallelizationTests.cpp
//
// Unit tests for InfinityLearn's parallelization features that use oneTBB.
// These tests verify that oneTBB can be included and used, and that the InfinityLearn wrappers
// function as expected.
//----------------------------------------------------------------------------------------------

#include "Core/Parallelization.h"       // parallelFor, parallelReduce, etc
#include <atomic>                       // std::atomic
#include <catch2/catch_approx.hpp>      // Catch::Approx.
#include <catch2/catch_test_macros.hpp> // TEST_CASE, REQUIRE
#include <cstddef>                      // std::size_t
#include <limits>                       // std::numeric_limits
#include <mutex>                        // std::mutex, std::lock_guard
#include <oneapi/tbb/blocked_range.h>   // oneapi::tbb::blocked_range
#include <oneapi/tbb/info.h>            // oneapi::tbb::info
#include <oneapi/tbb/parallel_for.h>    // oneapi::tbb::parallel_for
#include <oneapi/tbb/parallel_reduce.h> // oneapi::tbb::parallel_reduce
#include <set>                          // std::set
#include <thread>                       // std::this_thread::get_id
#include <vector>                       // std::vector

namespace
{
constexpr std::size_t kNumValues = 10000;
}

using namespace oneapi::tbb;
namespace ILP = InfinityLearn::Parallel;

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
    for (auto& c : visit_counts)
    {
        c.store(0, std::memory_order_relaxed);
    }

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
    for (auto& c : visit_counts)
    {
        c.store(0, std::memory_order_relaxed);
    }

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

//----------------------------------------------------------------------------------------------
// InfinityLearn::Parallel utilities tests
//----------------------------------------------------------------------------------------------

TEST_CASE("Parallel::Range reports size and empty state correctly", "[Core][Parallelization]")
{
    {
        const ILP::Range range{0, 10};
        REQUIRE(range.size() == 10);
        REQUIRE_FALSE(range.empty());
    }

    {
        const ILP::Range range{5, 5};
        REQUIRE(range.size() == 0);
        REQUIRE(range.empty());
    }

    {
        const ILP::Range range{10, 5};
        REQUIRE(range.size() == 0);
        REQUIRE(range.empty());
    }
}

TEST_CASE("Parallel::parallelFor does nothing for an empty range", "[Core][Parallelization]")
{
    bool was_called = false;
    ILP::parallelFor(0, 0, [&](ILP::Range) { was_called = true; });
    REQUIRE_FALSE(was_called);
}

TEST_CASE("Parallel::isolatedParallelFor does nothing for an empty range", "[Core][Parallelization]")
{
    bool was_called = false;
    ILP::isolatedParallelFor(0, 0, [&](ILP::Index) { was_called = true; });
    REQUIRE_FALSE(was_called);
}

TEST_CASE("Parallel::parallelFor visits every index exactly once through blocks", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 100000;
    std::vector<std::atomic<int>> visit_counts(num_values);
    for (auto& c : visit_counts)
    {
        c.store(0, std::memory_order_relaxed);
    }

    ILP::parallelFor(0, num_values,
                     [&](ILP::Range range)
                     {
                         REQUIRE(range.begin < range.end);
                         REQUIRE(range.end <= num_values);

                         for (ILP::Index i = range.begin; i < range.end; ++i)
                         {
                             visit_counts[i].fetch_add(1, std::memory_order_relaxed);
                         }
                     });

    for (ILP::Index i = 0; i < num_values; ++i)
    {
        REQUIRE(visit_counts[i].load(std::memory_order_relaxed) == 1);
    }
}

TEST_CASE("Parallel::isolatedParallelFor visits every index exactly once", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 100000;
    std::vector<std::atomic<int>> visit_counts(num_values);
    for (auto& c : visit_counts)
    {
        c.store(0, std::memory_order_relaxed);
    }

    ILP::isolatedParallelFor(0, num_values,
                             [&](ILP::Index i) { visit_counts[i].fetch_add(1, std::memory_order_relaxed); });

    for (ILP::Index i = 0; i < num_values; ++i)
    {
        REQUIRE(visit_counts[i].load(std::memory_order_relaxed) == 1);
    }
}

TEST_CASE("Parallel::isolatedParallelFor correctly writes element-wise values", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 100000;
    std::vector<int> values(num_values, 0);

    ILP::isolatedParallelFor(0, num_values, [&](ILP::Index i) { values[i] = static_cast<int>(i * 2); });

    for (ILP::Index i = 0; i < num_values; ++i)
    {
        REQUIRE(values[i] == static_cast<int>(i * 2));
    }
}

TEST_CASE("Parallel::parallelFor correctly writes element-wise values through blocks", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 100000;
    std::vector<int> values(num_values, 0);

    ILP::parallelFor(0, num_values,
                     [&](ILP::Range range)
                     {
                         for (ILP::Index i = range.begin; i < range.end; ++i)
                         {
                             values[i] = static_cast<int>(i + 7);
                         }
                     });

    for (ILP::Index i = 0; i < num_values; ++i)
    {
        REQUIRE(values[i] == static_cast<int>(i + 7));
    }
}

TEST_CASE("Parallel::parallelFor handles small ranges correctly", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 32;
    std::vector<int> values(num_values, 0);

    ILP::parallelFor(0, num_values,
                     [&](ILP::Range range)
                     {
                         for (ILP::Index i = range.begin; i < range.end; ++i)
                         {
                             values[i] = 1;
                         }
                     });

    for (ILP::Index i = 0; i < num_values; ++i)
    {
        REQUIRE(values[i] == 1);
    }
}

TEST_CASE("Parallel::isolatedParallelFor handles small ranges correctly", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 32;
    std::vector<int> values(num_values, 0);

    ILP::isolatedParallelFor(0, num_values, [&](ILP::Index i) { values[i] = 1; });

    for (ILP::Index i = 0; i < num_values; ++i)
    {
        REQUIRE(values[i] == 1);
    }
}

TEST_CASE("Parallel wrappers can execute work on multiple threads for large ranges", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 1000000;
    std::mutex mutex;
    std::set<std::thread::id> thread_ids;

    ILP::isolatedParallelFor(0, num_values,
                             [&](ILP::Index)
                             {
                                 const std::thread::id thread_id = std::this_thread::get_id();

                                 std::lock_guard<std::mutex> lock(mutex);
                                 thread_ids.insert(thread_id);
                             });

    REQUIRE_FALSE(thread_ids.empty());
    if (oneapi::tbb::info::default_concurrency() > 1)
    {
        REQUIRE(thread_ids.size() > 1);
    }
}

TEST_CASE("Parallel::parallelReduce computes integer sum", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 10000;
    const int sum = ILP::parallelReduce<int>(
        0, num_values, 0, [](ILP::Index i) { return static_cast<int>(i); }, [](int a, int b) { return a + b; });

    const int expected = static_cast<int>((num_values - 1) * num_values / 2);
    REQUIRE(sum == expected);
}

TEST_CASE("Parallel::parallelReduce returns identity for empty range", "[Core][Parallelization]")
{
    const int result =
        ILP::parallelReduce<int>(0, 0, 123, [](ILP::Index) { return 1; }, [](int a, int b) { return a + b; });

    REQUIRE(result == 123);
}

TEST_CASE("Parallel::parallelSum computes integer sum", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 10000;
    const int sum = ILP::parallelSum<int>(0, num_values, [](ILP::Index i) { return static_cast<int>(i); });

    const int expected = static_cast<int>((num_values - 1) * num_values / 2);
    REQUIRE(sum == expected);
}

TEST_CASE("Parallel::parallelSum computes double sum", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 10000;
    const double sum = ILP::parallelSum<double>(0, num_values, [](ILP::Index) { return 0.5; });

    REQUIRE(sum == Catch::Approx(5000.0));
}

TEST_CASE("Parallel::parallelSum returns zero for empty range", "[Core][Parallelization]")
{
    const int sum = ILP::parallelSum<int>(0, 0, [](ILP::Index) { return 100; });
    REQUIRE(sum == 0);
}

TEST_CASE("Parallel::parallelProduct computes integer product", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 6;
    const int product = ILP::parallelProduct<int>(0, num_values, [](ILP::Index i) { return static_cast<int>(i + 1); });

    REQUIRE(product == 720);
}

TEST_CASE("Parallel::parallelProduct returns one for empty range", "[Core][Parallelization]")
{
    const int product = ILP::parallelProduct<int>(0, 0, [](ILP::Index) { return 100; });
    REQUIRE(product == 1);
}

TEST_CASE("Parallel::parallelMin computes minimum value", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 10000;
    const int min_value =
        ILP::parallelMin<int>(0, num_values, [](ILP::Index i) { return static_cast<int>(5000) - static_cast<int>(i); });

    REQUIRE(min_value == -4999);
}

TEST_CASE("Parallel::parallelMax computes maximum value", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 10000;
    const int max_value = ILP::parallelMax<int>(0, num_values, [](ILP::Index i) { return static_cast<int>(i) - 5000; });

    REQUIRE(max_value == 4999);
}

TEST_CASE("Parallel::parallelMin returns max identity for empty range", "[Core][Parallelization]")
{
    const int min_value = ILP::parallelMin<int>(0, 0, [](ILP::Index) { return -100; });
    REQUIRE(min_value == std::numeric_limits<int>::max());
}

TEST_CASE("Parallel::parallelMax returns lowest identity for empty range", "[Core][Parallelization]")
{
    const int max_value = ILP::parallelMax<int>(0, 0, [](ILP::Index) { return 100; });
    REQUIRE(max_value == std::numeric_limits<int>::lowest());
}

TEST_CASE("Parallel::parallelMin computes floating-point minimum", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 10000;
    const float min_value =
        ILP::parallelMin<float>(0, num_values, [](ILP::Index i) { return static_cast<float>(i) * 0.25f - 100.0f; });

    REQUIRE(min_value == Catch::Approx(-100.0f));
}

TEST_CASE("Parallel::parallelMax computes floating-point maximum", "[Core][Parallelization]")
{
    constexpr ILP::Index num_values = 10000;
    const double max_value =
        ILP::parallelMax<double>(0, num_values, [](ILP::Index i) { return static_cast<double>(i) * 0.5; });

    REQUIRE(max_value == Catch::Approx(4999.5));
}
