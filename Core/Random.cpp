//-----------------------------------------------------------------------------
// The implementation of the random utilities for Random.h
//-----------------------------------------------------------------------------

#include "Random.h"
#include "Error.h"
#include <type_traits>

namespace InfinityLearn
{

Xoshiro256::Xoshiro256(Seed seed)
{
    SplitMix64 splitmix(seed);

    m_state[0] = splitmix.next();
    m_state[1] = splitmix.next();
    m_state[2] = splitmix.next();
    m_state[3] = splitmix.next();
}

Xoshiro256::Result Xoshiro256::next()
{
    // xoshiro256**
    const std::uint64_t result = rotl(m_state[1] * 5, 7) * 9;
    const std::uint64_t t = m_state[1] << 17;

    m_state[2] ^= m_state[0];
    m_state[3] ^= m_state[1];
    m_state[1] ^= m_state[2];
    m_state[0] ^= m_state[3];

    m_state[2] ^= t;
    m_state[3] = rotl(m_state[3], 45);

    return result;
}

Xoshiro256::Result Xoshiro256::rotl(Result x, int k)
{
    return (x << k) | (x >> (64 - k));
}

Xoshiro256::Result Xoshiro256::SplitMix64::next()
{
    // This is the SplitMix64 algorithm, which is used to initialize the state
    // of the Xoshiro256 generator. The magic numbers here are lifted from the
    // original SplitMix64 algorithm, and are chosen because they mix bits well
    // and have good statistical properties.
    Result z = (m_state += 0x9E3779B97F4A7C15ull);
    z = (z ^ (z >> 30)) * 0xBF58476D1CE4E5B9ull;
    z = (z ^ (z >> 27)) * 0x94D049BB133111EBull;
    return z ^ (z >> 31);
}

template <typename T>
T randomish(RNG& rng, T min, T max)
{
    IL_ASSERT((std::is_same<T, int>::value || std::is_same<T, float>::value ||
               std::is_same<T, double>::value),
              "randomish only supports int, float, and double types");

    if constexpr (std::is_same<T, int>::value)
    {
        const std::uint64_t range = static_cast<std::uint64_t>(max - min + 1);
        const std::uint64_t value = rng.next() % range;
        return static_cast<int>(min + static_cast<int>(value));
    }
    else if constexpr (std::is_same<T, float>::value ||
                       std::is_same<T, double>::value)
    {
        const T length =
            static_cast<T>(rng.next()) /
            static_cast<T>(std::numeric_limits<std::uint64_t>::max());
        return min + length * (max - min);
    }
}

}  // namespace InfinityLearn
