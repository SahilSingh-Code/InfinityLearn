//-----------------------------------------------------------------------------
// InfinityLearn / Core / TestUtils / TestRandom
//
// Test-only helpers for deterministic random number generation.
//-----------------------------------------------------------------------------

#pragma once

#include <Core/Random.h> // InfinityLearn::RNG
#include <catch2/catch_all.hpp>
#include <string_view> // std::string_view

namespace InfinityLearn
{

/// <summary>
/// Converts a string into a stable 64-bit RNG seed.
///
/// This uses the 64-bit FNV-1a hash algorithm. It is deterministic and stable
/// across platforms, which makes it useful for deriving repeatable random seeds
/// from unit test names.
/// </summary>
inline RNG::Seed hash_to_seed(std::string_view text)
{
    RNG::Seed hash = 14695981039346656037ull;

    for (const char character : text)
    {
        hash ^= static_cast<unsigned char>(character);
        hash *= 1099511628211ull;
    }

    return hash;
}

/// <summary>
/// Creates an RNG seeded from the current Catch2 test case name.
/// </summary>
inline RNG make_rng_for_current_test()
{
    return RNG(hash_to_seed(Catch::getResultCapture().getCurrentTestName()));
}

} // namespace InfinityLearn

/// <summary>
/// Creates a deterministic RNG seeded from the current Catch2 test case name.
/// </summary>
#define IL_RNG() ::InfinityLearn::make_rng_for_current_test()
