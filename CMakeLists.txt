#--------------------------------------------------------------------------------------------
# InfinityLearn
#
# Author: Sahil Singh
# Date: January 2026
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