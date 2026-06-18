#--------------------------------------------------------------------------------------------
# InfinityLearn / Core
#
# Author: Sahil Singh
# Date: June 2026
#
# This project contains the core functionalities of the InfinityLearn project.
#--------------------------------------------------------------------------------------------

# Define the Core project
project(Core)

# Create a static library for the Core module
add_library(Core STATIC
    Error.cpp Error.h
    Logging.cpp Logging.h
    Parallelization.h
    Random.cpp Random.h
    Timing.h)

# Include the current source directory for header files
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the Core library with the InfinityLearnConfig interface library to inherit the compile definitions
target_link_libraries(Core
    PUBLIC
        InfinityLearnConfig
        TBB::tbb
)

# Add the TestUtils subdirectory for utility functions used in unit tests
add_subdirectory(TestUtils)

# Add the CoreTests subdirectory for unit tests related to the Core module
add_subdirectory(CoreTests)