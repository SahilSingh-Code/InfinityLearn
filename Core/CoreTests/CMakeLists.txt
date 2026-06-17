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
    "RandomishTests.cpp"
    "TimerTests.cpp"
)

target_link_libraries(CoreTests
    PRIVATE
        Core
        CoreTestUtils
        Catch2::Catch2WithMain
)

include(Catch)
catch_discover_tests(CoreTests)