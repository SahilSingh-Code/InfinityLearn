#--------------------------------------------------------------------------------------------
# InfinityLearn / Core / CoreTests
#
# Author: Sahil Singh
# Date: June 2026
#
# This project contains unit tests for the Core module of the InfinityLearn project.
# These tests are designed to validate the functionality of the Core module and ensure that
# it behaves as expected under various conditions.
#--------------------------------------------------------------------------------------------

add_executable(CoreTests
    ParallelizationTests.cpp
    RandomishTests.cpp
    TimerTests.cpp
)

target_link_libraries(CoreTests
    PRIVATE
        Core
        CoreTestUtils
        Catch2::Catch2WithMain
        TBB::tbb
)

# Copy runtime DLL dependencies, such as oneTBB's tbb12_debug.dll/tbb12.dll,
# beside CoreTests.exe so Catch2 can run the executable during test discovery.
add_custom_command(
    TARGET CoreTests
    POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
        $<TARGET_RUNTIME_DLLS:CoreTests>
        $<TARGET_FILE_DIR:CoreTests>
    COMMAND_EXPAND_LISTS
)


include(Catch)
catch_discover_tests(CoreTests
    DISCOVERY_MODE PRE_TEST
)