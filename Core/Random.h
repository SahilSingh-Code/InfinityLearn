//-----------------------------------------------------------------------------
// This is InfinityLearn's random generation code. It provides functions for
// generating random numbers using the Xoshiro256** algorithm.
//-----------------------------------------------------------------------------
#pragma once

#include <cstdint>

namespace InfinityLearn
{

/// <summary>
/// A 256-bit Xoshiro random number generator.
/// </summary>
class Xoshiro256
{
   public:
    // Useful aliases for the seed and result types, which are both 64-bit
    // unsigned integers
    using Seed = std::uint64_t;
    using Result = std::uint64_t;

    /// <summary>
    /// Constructs a new Xoshiro256 random number generator.
    /// </summary>
    /// <param name="seed">The seed value to initialize the generator.</param>
    Xoshiro256(Seed seed);

    /// <summary>
    /// Generates the next random number.
    /// </summary>
    /// <returns>The next random number.</returns>
    Result next();

   private:
    class SplitMix64
    {
       public:
        /// <summary>
        /// Constructs a new SplitMix64 random number generator.
        /// </summary>
        /// <param name="seed">The seed value to initialize the
        /// generator.</param>
        explicit SplitMix64(std::uint64_t seed) : m_state(seed) {};

        /// <summary>
        /// Generates the next random number.
        /// </summary>
        /// <returns>The next random number.</returns>
        Result next();

       private:
        Result m_state;
    };

    /// <summary>
    /// Rotates the bits of a 64-bit integer to the left.
    /// </summary>
    /// <param name="x">The value to rotate.</param>
    /// <param name="k">The number of bits to rotate.</param>
    /// <returns>The rotated value.</returns>
    Result rotl(Result x, int k);

    Result m_state[4];
};

// Easy to remember and use alias
using RNG = Xoshiro256;

/// <summary>
/// Generate a random number between min and max, inclusive. The distribution is
/// not perfectly uniform, but it is good enough for most purposes. If no
/// min/max is provided, will generate a number between 0 and 1, inclusive.
/// </summary>
/// <param name="min">The start of the number generation range</param>
/// <param name="max">The end of the number generation range</param>
/// <param name="rng">The random number generator to use</param>
/// <returns>A random number in the designated range</returns>
template <typename T>
T randomish(RNG& rng, T min = 0, T max = 1);

}  // namespace InfinityLearn
