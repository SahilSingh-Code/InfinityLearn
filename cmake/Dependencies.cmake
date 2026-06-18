#--------------------------------------------------------------------------------------------
# Dependencies.cmake
#
# Fetches and configures third-party dependencies for InfinityLearn.
#--------------------------------------------------------------------------------------------

include(FetchContent)

function(iflearn_fetch_dependency dependency_name repository tag)
    FetchContent_Declare(
        ${dependency_name}
        GIT_REPOSITORY ${repository}
        GIT_TAG        ${tag}
    )

    FetchContent_MakeAvailable(${dependency_name})
endfunction()

#--------------------------------------------------------------------------------------------
# Catch2
#--------------------------------------------------------------------------------------------

iflearn_fetch_dependency(
    Catch2
    https://github.com/catchorg/Catch2.git
    v3.15.1
)

# Make Catch2's CMake helper scripts available.
# This gives us catch_discover_tests(...).
list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")

include(CTest)
include(Catch)

enable_testing()

#--------------------------------------------------------------------------------------------
# oneTBB
#--------------------------------------------------------------------------------------------

set(TBB_TEST OFF CACHE BOOL "" FORCE)
set(TBB_EXAMPLES OFF CACHE BOOL "" FORCE)
set(TBB_STRICT OFF CACHE BOOL "" FORCE)

iflearn_fetch_dependency(
    oneTBB
    https://github.com/oneapi-src/oneTBB.git
    v2023.0.0
)