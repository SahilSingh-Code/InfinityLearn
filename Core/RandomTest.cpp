//-----------------------------------------------------------------------------
// RandomSmokeTest.cpp
//
// Basic smoke test executable for InfinityLearn's random generation.
//-----------------------------------------------------------------------------

#include "Random.h"
#include <cassert>
#include <iostream>
#include <vector>

int main()
{
    using namespace InfinityLearn;

    std::cout << "Running RNG smoke tests...\n";

    // -------------------------------------------------------------------------
    // Test 1: Generator produces values without crashing
    // -------------------------------------------------------------------------
    {
        RNG rng(12345);

        const auto value = rng.next();

        std::cout << "First value from seed 12345: " << value << "\n";
    }

    // -------------------------------------------------------------------------
    // Test 2: Consecutive calls should not all return the same value
    // -------------------------------------------------------------------------
    {
        RNG rng(12345);

        constexpr int num_values = 16;

        std::vector<RNG::Result> values;
        values.reserve(num_values);

        for (int i = 0; i < num_values; ++i)
        {
            values.push_back(rng.next());
        }

        bool found_different_value = false;

        for (int i = 1; i < num_values; ++i)
        {
            if (values[i] != values[0])
            {
                found_different_value = true;
                break;
            }
        }

        assert(found_different_value &&
               "RNG generated identical values repeatedly.");

        std::cout << "Generated sequence:\n";

        for (const auto value : values)
        {
            std::cout << "    " << value << "\n";
        }
    }

    // -------------------------------------------------------------------------
    // Test 3: Same seed should generate the same sequence
    // -------------------------------------------------------------------------
    {
        RNG rng_a(12345);
        RNG rng_b(12345);

        constexpr int num_values = 16;

        for (int i = 0; i < num_values; ++i)
        {
            const auto a = rng_a.next();
            const auto b = rng_b.next();

            assert(a == b && "Same seed did not generate the same sequence.");
        }

        std::cout << "Same seed reproducibility smoke test passed.\n";
    }

    // -------------------------------------------------------------------------
    // Test 4: Different seeds should probably generate different sequences
    // -------------------------------------------------------------------------
    {
        RNG rng_a(12345);
        RNG rng_b(54321);

        constexpr int num_values = 16;

        bool found_difference = false;

        for (int i = 0; i < num_values; ++i)
        {
            const auto a = rng_a.next();
            const auto b = rng_b.next();

            if (a != b)
            {
                found_difference = true;
                break;
            }
        }

        assert(found_difference &&
               "Different seeds generated the same sequence.");

        std::cout << "Different seed smoke test passed.\n";
    }

    std::cout << "All RNG smoke tests passed.\n";

    return 0;
}
