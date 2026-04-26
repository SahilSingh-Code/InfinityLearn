#--------------------------------------------------------------------------------------------
# InfinityLearn / Core
#
# Author: Sahil Singh
# Date: January 2026
#
# This project contains the core functionalities of the InfinityLearn project.
#--------------------------------------------------------------------------------------------

# Define the Core project
project(Core)

# Create a static library for the Core module
add_library(Core STATIC
    Logging.cpp
    Logging.h
)

# Include the current source directory for header files
target_include_directories(Core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Define the log level based on the build configuration
target_compile_definitions(Core PRIVATE
    $<$<CONFIG:Debug>:IFLEARN_ACTIVE_LOG_LEVEL=DEBUG>
    $<$<CONFIG:Develop>:IFLEARN_ACTIVE_LOG_LEVEL=INFO>
    $<$<CONFIG:Release>:IFLEARN_ACTIVE_LOG_LEVEL=INFO>
    $<$<CONFIG:Performance>:IFLEARN_ACTIVE_LOG_LEVEL=PERFORMANCE>
)

# ---- Test executable ----
# This will just call the core logging functionality to ensure it works as expected.
# This is not a full unit test suite, but it serves as a basic sanity check for the logging system
add_executable(CoreLoggingTest
    LoggingTest.cpp
)

target_link_libraries(CoreLoggingTest
    PRIVATE Core
)