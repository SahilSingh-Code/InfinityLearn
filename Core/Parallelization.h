//-----------------------------------------------------------------------------------------------------------------------
// Parallelization.h
//
// Core parallelization utilities for InfinityLearn.
//
// This file provides small wrapper functions around oneTBB so the rest of InfinityLearn can use project-owned
// parallelization primitives instead of depending directly on oneTBB calls.
//-----------------------------------------------------------------------------------------------------------------------

#pragma once

#include "Error.h"                      // IL_ASSERT
#include <algorithm>                    // std::max
#include <cstddef>                      // std::size_t
#include <limits>                       // std::numeric_limits
#include <oneapi/tbb/blocked_range.h>   // oneapi::tbb::blocked_range
#include <oneapi/tbb/parallel_for.h>    // oneapi::tbb::parallel_for
#include <oneapi/tbb/parallel_reduce.h> // oneapi::tbb::parallel_reduce


namespace InfinityLearn
{

/// <summary>
/// Parallel execution utilities for the InfinityLearn project. These are small wrapper functions around oneTBB that
/// provide a simple interface for parallel execution and reduction across the project, and allow us to avoid depending
/// directly on oneTBB in the rest of the codebase.
/// </summary>
namespace Parallel
{

using Index = std::size_t;

/// <summary>
/// A data struct representing a half open range [begin, end)
/// </summary>
struct Range
{
    Index begin = 0;
    Index end = 0;

    [[nodiscard]] Index size() const noexcept
    {
        return (begin < end) ? (end - begin) : 0;
    }

    [[nodiscard]] bool empty() const noexcept
    {
        return begin >= end;
    }
};

//-----------------------------------------------------------------------------------------------------------------------
// Default tuning constants.
//
// kDefaultGrainSize:
//     The approximate minimum amount of work per TBB block.
//
// kDefaultParallelThreshold:
//     Small ranges execute serially to avoid parallel scheduling overhead. This is useful when we don't know how large
//     the workload will be, and we don't want to accidentally parallelize small tight loops that would be faster to run
//     serially. This threshold can be tuned based on the expected workload and the overhead of parallelization.
//-----------------------------------------------------------------------------------------------------------------------
inline constexpr Index kDefaultGrainSize = 1024;
inline constexpr Index kDefaultParallelThreshold = 4096;

//------------------------------------------------------------------------------------------
// Parallelization function declarations
//------------------------------------------------------------------------------------------

/// <summary>
/// Blocked parallel for loop that executes the given function in parallel over the range [begin, end). The function
/// should take a block as input , which is a half-open range of indices to process. The function will be called once
/// for each block, and the blocks need to be processed independently.
/// </summary>
/// <typeparam name="Func">A compiler-deduced function type, should not be needed to be explicitly declared.</typeparam>
/// <param name="begin">The start of the range to be parallelized.</param>
/// <param name="end">The end of the range to be parallelized.</param>
/// <param name="func">The function to be executed for each block. The function must take type Parallel::Range as
/// input.</param>
template <typename Func>
void parallelFor(Index begin, Index end, Func&& func);

/// <summary>
/// Parallel for loop that executes the given function in parallel The function should take a single index of type
/// Parallel::Index as input, and the loop will be parallelized across the range [begin, end).
/// </summary>
/// <typeparam name="Func">A compiler-deduced function type, should not be needed to be explicitly declared.</typeparam>
/// <param name="begin">The start of the range to be parallelized.</param>
/// <param name="end">The end of the range to be parallelized.</param>
/// <param name="func">The function to be executed for each block. The function must take type Parallel::Index as
/// input.</param>
template <typename Func>
void isolatedParallelFor(Index begin, Index end, Func&& func);

/// <summary>
/// A parallel reduction function that executes the given function in parallel over the range [begin, end) and reduces
/// the results using the given reduction function. The function should take an index of type Parallel::Index as input.
/// The reduction function should take two values of type T as input and return a value of type T, which is the result
/// of reducing the two input values.
/// </summary>
/// <typeparam name="T">What type of value is being reduced.</typeparam>
/// <typeparam name="Func">A compiler-deduced function type, should not be needed to be explicitly declared.</typeparam>
///
/// <typeparam name="ReduceFunc">A compiler-deduced function type, should not be needed to be explicitly
/// declared.</typeparam>
///
/// <param name="begin">The start of the range to be reduced.</param>
/// <param name="end">The end of the range to be reduced.</param>
/// <param name="identity">The identity value for the reduction.</param>
/// <param name="func">The function to be executed for each element. The function must take type Parallel::Index as
/// input.</param>
///
/// <param name="reduce_func">The function used to reduce the values. The function must take two values of type T as
/// input and return a value of type T.</param> <returns></returns>
template <typename T, typename Func, typename ReduceFunc>
T parallelReduce(Index begin, Index end, T identity, Func&& func, ReduceFunc&& reduce_func);

/// <summary>
/// Parallel sum over the half-open range [begin, end).
/// The function should take a Parallel::Index as input and return a value of type T.
/// </summary>
/// <typeparam name="T">The type of value being summed.</typeparam>
/// <typeparam name="Func">A compiler-deduced function type. This should not need to be explicitly
/// specified.</typeparam>
/// <param name="begin">The start of the range to be summed.</param>
/// <param name="end">The end of the range to be summed.</param>
/// <param name="func">The function to execute for each index. The function must take a Parallel::Index as input and
/// return a value of type T.</param>
/// <returns>The sum of all values returned by func.</returns>
template <typename T, typename Func>
T parallelSum(Index begin, Index end, Func&& func);

/// <summary>
/// Parallel product over the half-open range [begin, end).
/// The function should take a Parallel::Index as input and return a value of type T.
/// </summary>
/// <typeparam name="T">The type of value being multiplied.</typeparam>
/// <typeparam name="Func">A compiler-deduced function type. This should not need to be explicitly
/// specified.</typeparam>
/// <param name="begin">The start of the range to be multiplied.</param>
/// <param name="end">The end of the range to be multiplied.</param>
/// <param name="func">The function to execute for each index. The function must take a Parallel::Index as input and
/// return a value of type T.</param>
/// <returns>The product of all values returned by func.</returns>
template <typename T, typename Func>
T parallelProduct(Index begin, Index end, Func&& func);

/// <summary>
/// Parallel minimum over the half-open range [begin, end).
/// The function should take a Parallel::Index as input and return a value of type T.
/// </summary>
/// <typeparam name="T">The type of value being minimized.</typeparam>
/// <typeparam name="Func">A compiler-deduced function type. This should not need to be explicitly
/// specified.</typeparam>
/// <param name="begin">The start of the range to be minimized.</param>
/// <param name="end">The end of the range to be minimized.</param>
/// <param name="func">The function to execute for each index. The function must take a Parallel::Index as input and
/// return a value of type T.</param>
/// <returns>The minimum value returned by func.</returns>
template <typename T, typename Func>
T parallelMin(Index begin, Index end, Func&& func);

/// <summary>
/// Parallel maximum over the half-open range [begin, end).
/// The function should take a Parallel::Index as input and return a value of type T.
/// </summary>
/// <typeparam name="T">The type of value being maximized.</typeparam>
/// <typeparam name="Func">A compiler-deduced function type. This should not need to be explicitly
/// specified.</typeparam>
/// <param name="begin">The start of the range to be maximized.</param> <param name="end">The end of the range to be
/// maximized.</param>
/// <param name="func">The function to execute for each index. The function must take a Parallel::Index as input and
/// return a value of type T.</param>
/// <returns>The maximum value returned by func.</returns>
template <typename T, typename Func>
T parallelMax(Index begin, Index end, Func&& func);


//-----------------------------------------------------------------------------------------------------------------------
// Parallelization function definitions
//
// This is necessary for the template functions, which need to be defined in the header file.
//-----------------------------------------------------------------------------------------------------------------------

template <typename Func>
void parallelFor(Index begin, Index end, Func&& func)
{
    IL_ASSERT(begin <= end, "parallelFor requires begin <= end (begin=" << begin << ", end=" << end << ")");

    if (begin == end)
    {
        return;
    }

    const Index count = end - begin;
    if (count < kDefaultParallelThreshold)
    {
        func(Range{begin, end});
        return;
    }

    oneapi::tbb::parallel_for(oneapi::tbb::blocked_range<Index>(begin, end, kDefaultGrainSize),
                              [&](const oneapi::tbb::blocked_range<Index>& range)
                              { func(Range{range.begin(), range.end()}); });
}

template <typename Func>
void isolatedParallelFor(Index begin, Index end, Func&& func)
{
    IL_ASSERT(begin <= end, "isolatedParallelFor requires begin <= end (begin=" << begin << ", end=" << end << ")");

    if (begin == end)
    {
        return;
    }

    const Index count = end - begin;
    if (count < kDefaultParallelThreshold)
    {
        for (Index i = begin; i < end; ++i)
        {
            func(i);
        }

        return;
    }

    oneapi::tbb::parallel_for(oneapi::tbb::blocked_range<Index>(begin, end, kDefaultGrainSize),
                              [&](const oneapi::tbb::blocked_range<Index>& range)
                              {
                                  for (Index i = range.begin(); i < range.end(); ++i)
                                  {
                                      func(i);
                                  }
                              });
}

template <typename T, typename Func, typename ReduceFunc>
T parallelReduce(Index begin, Index end, T identity, Func&& func, ReduceFunc&& reduce_func)
{
    if (begin >= end)
    {
        return identity;
    }

    const Index count = end - begin;
    if (count < kDefaultParallelThreshold)
    {
        T result = identity;
        for (Index i = begin; i < end; ++i)
        {
            result = reduce_func(result, func(i));
        }

        return result;
    }

    return oneapi::tbb::parallel_reduce(
        oneapi::tbb::blocked_range<Index>(begin, end, kDefaultGrainSize), identity,
        [&](const oneapi::tbb::blocked_range<Index>& range, T local_result)
        {
            for (Index i = range.begin(); i < range.end(); ++i)
            {
                local_result = reduce_func(local_result, func(i));
            }

            return local_result;
        },
        reduce_func);
}

template <typename T, typename Func>
T parallelSum(Index begin, Index end, Func&& func)
{
    return parallelReduce<T>(begin, end, T{0}, func, [](T a, T b) { return a + b; });
}

template <typename T, typename Func>
T parallelProduct(Index begin, Index end, Func&& func)
{
    return parallelReduce<T>(begin, end, T{1}, func, [](T a, T b) { return a * b; });
}

template <typename T, typename Func>
T parallelMin(Index begin, Index end, Func&& func)
{
    return parallelReduce<T>(begin, end, std::numeric_limits<T>::max(), func, [](T a, T b) { return std::min(a, b); });
}

template <typename T, typename Func>
T parallelMax(Index begin, Index end, Func&& func)
{
    return parallelReduce<T>(begin, end, std::numeric_limits<T>::lowest(), func,
                             [](T a, T b) { return std::max(a, b); });
}

} // namespace Parallel
} // namespace InfinityLearn
