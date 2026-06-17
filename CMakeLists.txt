#--------------------------------------------------------------------------------------------
# InfinityLearn
#
# Author: Sahil Singh
# Date: June 2026
#
# This CMakeLists describes the overall project configuration for the InfinityLearn project.
#--------------------------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.10)

# Define the supported build configurations
set(CMAKE_CONFIGURATION_TYPES "Debug;Develop;Release;Performance" CACHE STRING "" FORCE)

# Set the default build type to Develop if not specified
if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Develop CACHE STRING "Build type" FORCE)
endif()

# Set the available build types for the project
set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    Debug
    Develop
    Release
    Performance
)

# Enable hot reload (edit and continue) if available
include (cmake/HotReload.cmake)

# Main project configuration
project ("InfinityLearn")

# Set the C++ standard for the project
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Add Catch2 and enable testing for the project
include(FetchContent)

FetchContent_Declare(
    Catch2
    GIT_REPOSITORY https://github.com/catchorg/Catch2.git
    GIT_TAG v3.15.1
)

FetchContent_MakeAvailable(Catch2)

# Make Catch2's CMake helper scripts available.
# This gives us catch_discover_tests(...).
list(APPEND CMAKE_MODULE_PATH "${catch2_SOURCE_DIR}/extras")

include(CTest)
include(Catch)

enable_testing()

# Create a library to contain the common configuration for the project.
# This will allow us to define compile definitions and other settings that can be shared across all sub-projects.
add_library(InfinityLearnConfig INTERFACE)
target_compile_definitions(InfinityLearnConfig INTERFACE
    $<$<CONFIG:Debug>:IL_BUILD_TYPE="Debug">
    $<$<CONFIG:Develop>:IL_BUILD_TYPE="Develop">
    $<$<CONFIG:Release>:IL_BUILD_TYPE="Release">
    $<$<CONFIG:Performance>:IL_BUILD_TYPE="Performance">

    $<$<CONFIG:Debug>:IL_ENABLE_ASSERTS=1>
    $<$<CONFIG:Develop>:IL_ENABLE_ASSERTS=1>
    $<$<CONFIG:Release>:IL_ENABLE_ASSERTS=1>
    $<$<CONFIG:Performance>:IL_ENABLE_ASSERTS=0>

    $<$<CONFIG:Debug>:IL_ENABLE_DEBUG_ASSERTS=1>
    $<$<CONFIG:Develop>:IL_ENABLE_DEBUG_ASSERTS=1>
    $<$<CONFIG:Release>:IL_ENABLE_DEBUG_ASSERTS=0>
    $<$<CONFIG:Performance>:IL_ENABLE_DEBUG_ASSERTS=0>

    $<$<CONFIG:Debug>:IFLEARN_ACTIVE_LOG_LEVEL=DEBUG>
    $<$<CONFIG:Develop>:IFLEARN_ACTIVE_LOG_LEVEL=INFO>
    $<$<CONFIG:Release>:IFLEARN_ACTIVE_LOG_LEVEL=INFO>
    $<$<CONFIG:Performance>:IFLEARN_ACTIVE_LOG_LEVEL=PERFORMANCE>
)

# Sub-projects
add_subdirectory ("Core")