#--------------------------------------------------------------------------------------------
# InfinityLearn
#
# Author: Sahil Singh
# Date: June 2026
#
# This CMakeLists describes the overall project configuration for the InfinityLearn project.
#--------------------------------------------------------------------------------------------
cmake_minimum_required(VERSION 3.14)

project("InfinityLearn")

# Project-wide CMake setup
include(cmake/BuildConfigurations.cmake)
include(cmake/HotReload.cmake)
include(cmake/Dependencies.cmake)
include(cmake/ProjectConfig.cmake)

# Sub-projects
add_subdirectory(Core)