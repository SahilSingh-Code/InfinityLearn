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
    VectorTests.cpp
)

target_link_libraries(MathTests
    PRIVATE
        Core
        CoreTestUtils
        Catch2::Catch2WithMain
)

# Copy runtime DLL dependencies, such as oneTBB's tbb12_debug.dll/tbb12.dll,
# beside MathTests.exe so Catch2 can run the executable during test discovery.
if(WIN32)
    add_custom_command(TARGET MathTests POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy_if_different
            $<TARGET_RUNTIME_DLLS:MathTests>
            $<TARGET_FILE_DIR:MathTests>
        COMMAND_EXPAND_LISTS
    )
endif()

include(Catch)
catch_discover_tests(MathTests)