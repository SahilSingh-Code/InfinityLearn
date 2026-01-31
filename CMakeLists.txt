#--------------------------------------------------------------------------------------------
# InfinityLearn
#
# Author: Sahil Singh
# Date: January 2026
#
# This CMakeLists describes the overall project configuration for the InfinityLearn project.
#--------------------------------------------------------------------------------------------
cmake_minimum_required (VERSION 3.10)

 # Enable hot reload (edit and continue) if available
 include (cmake/HotReload.cmake)

# Main project configuration
project ("InfinityLearn")

# Sub-projects
add_subdirectory ("Core")