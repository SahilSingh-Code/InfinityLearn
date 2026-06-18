#--------------------------------------------------------------------------------------------
# InfinityLearn / Math / MathTests
#
# Author: Sahil Singh
# Date: June 2026
#
# This project contains unit tests for the Math module of the InfinityLearn project.
# These tests are designed to validate the functionality of the Math module and ensure that
# it behaves as expected under various conditions.
#--------------------------------------------------------------------------------------------

add_executable(MathTests
)

target_link_libraries(MathTests
    PRIVATE
        Core
        CoreTestUtils
)

include(Catch)
catch_discover_tests(MathTests)