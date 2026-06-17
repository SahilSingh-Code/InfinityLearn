#include <Core/Timing.h>                // InfinityLearn::Timer and InfinityLearn::ScopedTimer
#include <catch2/catch_test_macros.hpp> // TEST_CASE and REQUIRE/REQUIRE_NOTHROW
#include <chrono>                       // std::chrono::milliseconds
#include <thread>                       // std::this_thread::sleep_for
#include <type_traits>                  // std::is_copy_constructible_v, etc.

namespace
{
using InfinityLearn::ScopedTimer;
using InfinityLearn::Timer;

TEST_CASE("Timer starts immediately after construction", "[Core][Timer]")
{
    const Timer timer;
    const double elapsed = timer.elapsed();
    REQUIRE(elapsed >= 0.0);
}

TEST_CASE("Timer elapsed time increases as time passes", "[Core][Timer]")
{
    Timer timer;
    const double before_sleep = timer.elapsed();
    std::this_thread::sleep_for(std::chrono::milliseconds(2));
    const double after_sleep = timer.elapsed();

    REQUIRE(after_sleep >= before_sleep);
    REQUIRE(after_sleep > 0.0);
}

TEST_CASE("Timer reset restarts elapsed time measurement", "[Core][Timer]")
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    const double elapsed_before_reset = timer.elapsed();
    timer.reset();
    const double elapsed_after_reset = timer.elapsed();

    REQUIRE(elapsed_before_reset > 0.0);
    REQUIRE(elapsed_after_reset >= 0.0);
    REQUIRE(elapsed_after_reset < elapsed_before_reset);
}

TEST_CASE("Timer reports elapsed time in seconds", "[Core][Timer]")
{
    Timer timer;
    std::this_thread::sleep_for(std::chrono::milliseconds(10));
    const double elapsed = timer.elapsed();

    // 10 ms is 0.01 seconds. Use a loose lower bound
    REQUIRE(elapsed > 0.001);

    // Use a loose upper bound to catch unit mistakes, such as returning
    // milliseconds instead of seconds.
    REQUIRE(elapsed < 1.0);
}

TEST_CASE("ScopedTimer can be constructed and destroyed without throwing", "[Core][ScopedTimer]")
{
    REQUIRE_NOTHROW([]() { ScopedTimer timer("ScopedTimer test"); }());
}

TEST_CASE("ScopedTimer handles a null name without throwing", "[Core][ScopedTimer]")
{
    REQUIRE_NOTHROW([]() { ScopedTimer timer(nullptr); }());
}

TEST_CASE("ScopedTimer is not copyable or movable", "[Core][ScopedTimer]")
{
    static_assert(!std::is_copy_constructible_v<ScopedTimer>);
    static_assert(!std::is_copy_assignable_v<ScopedTimer>);
    static_assert(!std::is_move_constructible_v<ScopedTimer>);
    static_assert(!std::is_move_assignable_v<ScopedTimer>);

    REQUIRE(true);
}

TEST_CASE("Timer is copyable by default", "[Core][Timer]")
{
    static_assert(std::is_copy_constructible_v<Timer>);
    static_assert(std::is_copy_assignable_v<Timer>);
    static_assert(std::is_move_constructible_v<Timer>);
    static_assert(std::is_move_assignable_v<Timer>);

    REQUIRE(true);
}
} // namespace
