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

# Sub-projects
add_subdirectory ("Core")