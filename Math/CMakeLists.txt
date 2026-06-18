#--------------------------------------------------------------------------------------------
# InfinityLearn / Math
#
# Author: Sahil Singh
# Date: June 2026
#
# This project contains all of the mathematical components of the InfinityLearn project,
# including linear algebra, optimization algorithms, and numerical methods.
#--------------------------------------------------------------------------------------------

# Define the Math project
project(Math)

# Create a static library for the Math module
add_library(Math STATIC
    Vectors.h
   )

# Include the current source directory for header files
target_include_directories(Math PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})

# Link the Core library with the InfinityLearnConfig interface library to inherit the compile definitions
target_link_libraries(Math
    PUBLIC
        Core
)
# Add the MathTests subdirectory for unit tests related to the Math module
add_subdirectory(MathTests)