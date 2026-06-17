#--------------------------------------------------------------------------------------------
# InfinityLearn / Core / TestUtils
#
# Author: Sahil Singh
# Date: June 2026
#
# Shared test utilities for Core unit tests.
#--------------------------------------------------------------------------------------------

add_library(CoreTestUtils INTERFACE)

target_include_directories(CoreTestUtils
    INTERFACE
        ${CMAKE_CURRENT_SOURCE_DIR}/../..
)

target_link_libraries(CoreTestUtils
    INTERFACE
        Core
        Catch2::Catch2WithMain
)