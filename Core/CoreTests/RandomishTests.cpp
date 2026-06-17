#include "Core/TestUtils/TestRandom.h"  // IL_RNG for deterministic RNGs in tests
#include "Random.h"                     // InfinityLearn::RNG and InfinityLearn::randomish
#include <algorithm>                    // std::sort and std::unique
#include <array>                        // std::array for integer distribution buckets
#include <catch2/catch_approx.hpp>      // Catch::Approx for approximate floating-point comparisons
#include <catch2/catch_test_macros.hpp> // TEST_CASE and REQUIRE
#include <numeric>                      // std::accumulate
#include <vector>                       // std::vector

namespace
{
using namespace InfinityLearn;

// Generate a vector of random values using the provided RNG and range parameters.
template <typename T>
std::vector<T> generate_values(RNG& rng, int count, T min_value, T max_value)
{
    std::vector<T> values;
    values.reserve(static_cast<std::size_t>(count));

    for (int i = 0; i < count; ++i)
    {
        values.push_back(randomish<T>(rng, min_value, max_value));
    }

    return values;
}

// Find the mean of a vector of values.
template <typename T>
double mean(const std::vector<T>& values)
{
    REQUIRE(!values.empty());

    const double sum = std::accumulate(values.begin(), values.end(), 0.0,
                                       [](double acc, T value) { return acc + static_cast<double>(value); });

    return sum / static_cast<double>(values.size());
}

// Find the variance of a vector of values.
template <typename T>
double variance(const std::vector<T>& values)
{
    REQUIRE(values.size() >= 2);

    const double m = mean(values);

    double sum_squared_differences = 0.0;

    for (const T value : values)
    {
        const double difference = static_cast<double>(value) - m;
        sum_squared_differences += difference * difference;
    }

    return sum_squared_differences / static_cast<double>(values.size());
}

// Count how many unique values there are in a vector of values.
template <typename T>
int count_unique_values(const std::vector<T>& values)
{
    std::vector<T> sorted = values;
    std::sort(sorted.begin(), sorted.end());

    const auto new_end = std::unique(sorted.begin(), sorted.end());

    return static_cast<int>(std::distance(sorted.begin(), new_end));
}
} // namespace

TEST_CASE("randomish<double> generates values in the default range [0, 1]", "[Core][randomish]")
{
    RNG rng(12345);

    for (int i = 0; i < 10000; ++i)
    {
        const double value = randomish<double>(rng);

        REQUIRE(value >= 0.0);
        REQUIRE(value <= 1.0);
    }
}

TEST_CASE("randomish<float> generates values in the default range [0, 1]", "[Core][randomish]")
{
    RNG rng(12345);

    for (int i = 0; i < 10000; ++i)
    {
        const float value = randomish<float>(rng);

        REQUIRE(value >= 0.0f);
        REQUIRE(value <= 1.0f);
    }
}

TEST_CASE("randomish<int> generates values in the default range [0, 1]", "[Core][randomish]")
{
    RNG rng(12345);

    for (int i = 0; i < 10000; ++i)
    {
        const int value = randomish<int>(rng);

        REQUIRE(value >= 0);
        REQUIRE(value <= 1);
    }
}

TEST_CASE("randomish<double> generates values within the requested range", "[Core][randomish]")
{
    RNG rng(12345);

    constexpr double min_value = -10.0;
    constexpr double max_value = 25.0;

    for (int i = 0; i < 10000; ++i)
    {
        const double value = randomish<double>(rng, min_value, max_value);

        REQUIRE(value >= min_value);
        REQUIRE(value <= max_value);
    }
}

TEST_CASE("randomish<float> generates values within the requested range", "[Core][randomish]")
{
    RNG rng(12345);

    constexpr float min_value = -3.5f;
    constexpr float max_value = 7.25f;

    for (int i = 0; i < 10000; ++i)
    {
        const float value = randomish<float>(rng, min_value, max_value);

        REQUIRE(value >= min_value);
        REQUIRE(value <= max_value);
    }
}

TEST_CASE("randomish<int> generates values within the requested inclusive range", "[Core][randomish]")
{
    RNG rng(12345);

    constexpr int min_value = -10;
    constexpr int max_value = 10;

    for (int i = 0; i < 10000; ++i)
    {
        const int value = randomish<int>(rng, min_value, max_value);

        REQUIRE(value >= min_value);
        REQUIRE(value <= max_value);
    }
}

TEST_CASE("randomish is deterministic for the same seed", "[Core][randomish]")
{
    RNG rng_a(12345);
    RNG rng_b(12345);

    for (int i = 0; i < 1000; ++i)
    {
        const double a = randomish<double>(rng_a, -1.0, 1.0);
        const double b = randomish<double>(rng_b, -1.0, 1.0);

        REQUIRE(a == b);
    }
}

TEST_CASE("randomish<int> is deterministic for the same seed", "[Core][randomish]")
{
    RNG rng_a(12345);
    RNG rng_b(12345);

    for (int i = 0; i < 1000; ++i)
    {
        const int a = randomish<int>(rng_a, -100, 100);
        const int b = randomish<int>(rng_b, -100, 100);

        REQUIRE(a == b);
    }
}

TEST_CASE("randomish produces different sequences for different seeds", "[Core][randomish]")
{
    RNG rng_a(12345);
    RNG rng_b(54321);

    bool found_difference = false;

    for (int i = 0; i < 100; ++i)
    {
        const double a = randomish<double>(rng_a, -1.0, 1.0);
        const double b = randomish<double>(rng_b, -1.0, 1.0);

        if (a != b)
        {
            found_difference = true;
            break;
        }
    }

    REQUIRE(found_difference);
}

TEST_CASE("randomish<double> produces non-constant values", "[Core][randomish]")
{
    RNG rng(12345);

    const auto values = generate_values<double>(rng, 1000, -1.0, 1.0);

    const int unique_count = count_unique_values(values);

    REQUIRE(unique_count > 900);
}

TEST_CASE("randomish<float> produces non-constant values", "[Core][randomish]")
{
    RNG rng(12345);

    const auto values = generate_values<float>(rng, 1000, -1.0f, 1.0f);

    const int unique_count = count_unique_values(values);

    REQUIRE(unique_count > 900);
}

TEST_CASE("randomish<int> produces multiple distinct values", "[Core][randomish]")
{
    RNG rng(12345);

    const auto values = generate_values<int>(rng, 1000, -10, 10);

    const int unique_count = count_unique_values(values);

    // There are only 21 possible values in [-10, 10], so duplicates are
    // expected. This verifies that the generator is not stuck on one value.
    REQUIRE(unique_count > 10);
}

TEST_CASE("randomish<int> can generate both values in the default int range", "[Core][randomish]")
{
    RNG rng(12345);

    bool generated_zero = false;
    bool generated_one = false;

    for (int i = 0; i < 1000; ++i)
    {
        const int value = randomish<int>(rng);

        if (value == 0)
        {
            generated_zero = true;
        }
        else if (value == 1)
        {
            generated_one = true;
        }
    }

    REQUIRE(generated_zero);
    REQUIRE(generated_one);
}

TEST_CASE("randomish<int> returns the only possible value when min equals max", "[Core][randomish]")
{
    RNG rng(12345);

    for (int i = 0; i < 100; ++i)
    {
        REQUIRE(randomish<int>(rng, 7, 7) == 7);
    }
}

TEST_CASE("randomish<float> returns the only possible value when min equals max", "[Core][randomish]")
{
    RNG rng(12345);

    for (int i = 0; i < 100; ++i)
    {
        REQUIRE(randomish<float>(rng, 3.5f, 3.5f) == 3.5f);
    }
}

TEST_CASE("randomish<double> returns the only possible value when min equals max", "[Core][randomish]")
{
    RNG rng(12345);

    for (int i = 0; i < 100; ++i)
    {
        REQUIRE(randomish<double>(rng, -2.25, -2.25) == -2.25);
    }
}

TEST_CASE("randomish<double> has approximately uniform mean over symmetric range", "[Core][randomish][distribution]")
{
    RNG rng(12345);

    constexpr int sample_count = 100000;
    constexpr double min_value = -1.0;
    constexpr double max_value = 1.0;

    const auto values = generate_values<double>(rng, sample_count, min_value, max_value);

    const double observed_mean = mean(values);

    // Uniform distribution over [-1, 1] has expected mean 0.
    // The tolerance is intentionally loose to avoid flaky tests.
    REQUIRE(observed_mean == Catch::Approx(0.0).margin(0.01));
}

TEST_CASE("randomish<double> has approximately expected variance", "[Core][randomish][distribution]")
{
    RNG rng(12345);

    constexpr int sample_count = 100000;
    constexpr double min_value = -1.0;
    constexpr double max_value = 1.0;

    const auto values = generate_values<double>(rng, sample_count, min_value, max_value);

    const double observed_variance = variance(values);

    // Uniform distribution over [a, b] has variance (b - a)^2 / 12.
    constexpr double expected_variance = 4.0 / 12.0;

    REQUIRE(observed_variance == Catch::Approx(expected_variance).margin(0.01));
}

TEST_CASE("randomish<float> has approximately uniform mean over positive range", "[Core][randomish][distribution]")
{
    RNG rng(12345);

    constexpr int sample_count = 100000;
    constexpr float min_value = 0.0f;
    constexpr float max_value = 10.0f;

    const auto values = generate_values<float>(rng, sample_count, min_value, max_value);

    const double observed_mean = mean(values);

    // Uniform distribution over [0, 10] has expected mean 5.
    REQUIRE(observed_mean == Catch::Approx(5.0).margin(0.05));
}

TEST_CASE("randomish<int> has approximately uniform bucket counts", "[Core][randomish][distribution]")
{
    RNG rng(12345);

    constexpr int min_value = 0;
    constexpr int max_value = 9;
    constexpr int bucket_count = max_value - min_value + 1;
    constexpr int sample_count = 100000;

    std::array<int, bucket_count> buckets{};

    for (int i = 0; i < sample_count; ++i)
    {
        const int value = randomish<int>(rng, min_value, max_value);

        REQUIRE(value >= min_value);
        REQUIRE(value <= max_value);

        buckets[static_cast<std::size_t>(value - min_value)]++;
    }

    constexpr double expected_count_per_bucket = static_cast<double>(sample_count) / static_cast<double>(bucket_count);

    for (const int count : buckets)
    {
        // 5% tolerance is intentionally loose.
        REQUIRE(static_cast<double>(count) == Catch::Approx(expected_count_per_bucket).epsilon(0.05));
    }
}

TEST_CASE("IL_RNG creates deterministic RNGs within the same test case", "[Core][randomish][rng]")
{
    auto rng_a = IL_RNG();
    auto rng_b = IL_RNG();

    for (int i = 0; i < 100; ++i)
    {
        REQUIRE(rng_a.next() == rng_b.next());
    }
}

TEST_CASE("IL_RNG can be used directly with randomish<double>", "[Core][randomish][IL_RNG]")
{
    auto rng = IL_RNG();

    constexpr double min_value = -5.0;
    constexpr double max_value = 5.0;

    const double value = randomish<double>(rng, min_value, max_value);

    REQUIRE(value >= min_value);
    REQUIRE(value <= max_value);
}

TEST_CASE("IL_RNG can be used directly with randomish<float>", "[Core][randomish][IL_RNG]")
{
    auto rng = IL_RNG();

    constexpr float min_value = 10.0f;
    constexpr float max_value = 20.0f;

    const float value = randomish<float>(rng, min_value, max_value);

    REQUIRE(value >= min_value);
    REQUIRE(value <= max_value);
}

TEST_CASE("IL_RNG can be used directly with randomish<int>", "[Core][randomish][IL_RNG]")
{
    auto rng = IL_RNG();

    constexpr int min_value = 100;
    constexpr int max_value = 200;

    const int value = randomish<int>(rng, min_value, max_value);

    REQUIRE(value >= min_value);
    REQUIRE(value <= max_value);
}

TEST_CASE("IL_RNG produces an RNG that can generate a sequence of randomish values", "[Core][randomish][IL_RNG]")
{
    auto rng = IL_RNG();

    constexpr int sample_count = 100;
    constexpr double min_value = -1.0;
    constexpr double max_value = 1.0;

    bool found_different_value = false;

    const double first_value = randomish<double>(rng, min_value, max_value);

    for (int i = 1; i < sample_count; ++i)
    {
        const double value = randomish<double>(rng, min_value, max_value);

        REQUIRE(value >= min_value);
        REQUIRE(value <= max_value);

        if (value != first_value)
        {
            found_different_value = true;
        }
    }

    REQUIRE(found_different_value);
}

TEST_CASE("IL_RNG creates deterministic RNGs for the same Catch2 test case name", "[Core][randomish][IL_RNG]")
{
    auto rng_a = IL_RNG();
    auto rng_b = IL_RNG();

    for (int i = 0; i < 100; ++i)
    {
        REQUIRE(rng_a.next() == rng_b.next());
    }
}

TEST_CASE("randomish advances the RNG state when the same RNG is reused", "[Core][randomish][IL_RNG]")
{
    auto rng = IL_RNG();

    constexpr double min_value = -1.0;
    constexpr double max_value = 1.0;

    const double first_value = randomish<double>(rng, min_value, max_value);
    const double second_value = randomish<double>(rng, min_value, max_value);
    const double third_value = randomish<double>(rng, min_value, max_value);

    REQUIRE(second_value != first_value);
    REQUIRE(third_value != first_value);
    REQUIRE(third_value != second_value);
}
