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
    Random.cpp Random.h
    Timing.h)

# Include the current source directory for header files
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the Core library with the InfinityLearnConfig interface library to inherit the compile definitions
target_link_libraries(Core
    PUBLIC InfinityLearnConfig
)

# Add the TestUtils subdirectory for utility functions used in unit tests
add_subdirectory(TestUtils)

# Add the CoreTests subdirectory for unit tests related to the Core module
add_subdirectory(CoreTests)

# ---- Test executables ----

# This will just call the core logging functionality to ensure it works as expected.
# This is not a full unit test suite, but it serves as a basic sanity check for the logging system
add_executable(CoreLoggingTest
    LoggingTest.cpp)

target_link_libraries(CoreLoggingTest
    PRIVATE Core
)

# This will test the error handling functionality of the Core module.
# It will trigger some errors and ensure that they are handled correctly.
# This is not a full unit test suite, but it serves as a basic sanity check for the error handling system.
add_executable(CoreErrorCheck
    ErrorTest.cpp)

target_link_libraries(CoreErrorCheck
    PRIVATE Core
)

# This will test the random number generation functionality of the Core module.
# This is not a full unit test suite, but it serves as a basic sanity check for the random number generation system.
add_executable(CoreRandomCheck
    RandomTest.cpp)

target_link_libraries(CoreRandomCheck
    PRIVATE Core
)