//------------------------------------------------------------------------------
// This is InfinityLearn's timing header file.
// It defines a timer class for measuring elapsed time in the InfinityLearn
// project.
//------------------------------------------------------------------------------

#ifndef INFINITYLEARN_TIMING_H
#define INFINITYLEARN_TIMING_H

#include "Logging.h"
#include <chrono>

namespace InfinityLearn
{

// A manual timing class that can be used to measure elapsed time for a block of
// code.
// Creating the timer starts the timer, and an elapsed time can be retrieved.
// The timer can also be reset when needed to start the timer again.
class Timer
{
   public:
    /// <summary>
    /// Create a timer and start timing immediately.
    /// </summary>
    Timer() : m_start_time(std::chrono::high_resolution_clock::now())
    {
    }

    /// <summary>
    /// Reset the timer to 0 and continue timing.
    /// </summary>
    void reset()
    {
        m_start_time = std::chrono::high_resolution_clock::now();
    }

    /// <summary>
    /// Figure out the elapsed time in seconds since the timer was started or
    /// last reset.
    /// </summary>
    /// <returns> The elpased time in seconds. </returns>
    double elapsed() const
    {
        return std::chrono::duration<double>(
                   std::chrono::high_resolution_clock::now() - m_start_time)
            .count();
    }

   private:
    std::chrono::high_resolution_clock::time_point m_start_time;
};

/// <summary>
/// A scoped timer that automatically logs the elapsed time when it goes out of
/// scope. This is useful for timing the execution of a block of code without
/// needing to manually call a logging function at the end of the block. The
/// timing starts immediately upon creation, and ends when the object goes out
/// of scope.
/// </summary>
class ScopedTimer
{
   public:
    /// <summary>
    /// Create a scoped timer that will automatically log the elapsed time when
    /// it goes out of scope. The timer starts immediately upon construction.
    /// </summary>
    /// <param name="name">The name of the scope to be displayed when
    /// logged</param>
    explicit ScopedTimer(const char* name) : m_timer(), m_name(name)
    {
    }

    /// <summary>
    /// When destroyed, log the elapsed time since the timer was created with
    /// the provided name. The log will be at the PERFORMANCE log level.
    /// </summary>
    ~ScopedTimer()
    {
        Log(LogLevel::PERFORMANCE, "Timing: " + std::string(m_name) + " took " +
                                       std::to_string(m_timer.elapsed()) +
                                       " seconds");
    }

   private:
    Timer m_timer;
    const char* m_name;
};
}  // namespace InfinityLearn

#endif  // INFINITYLEARN_TIMING_H
