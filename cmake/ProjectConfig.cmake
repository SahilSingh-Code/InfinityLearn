#--------------------------------------------------------------------------------------------
# ProjectConfig.cmake
#
# Defines shared compile definitions and project-wide configuration settings.
#--------------------------------------------------------------------------------------------

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

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