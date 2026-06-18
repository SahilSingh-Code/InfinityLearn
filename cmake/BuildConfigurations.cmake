#--------------------------------------------------------------------------------------------
# BuildConfigurations.cmake
#
# Defines the supported InfinityLearn build configurations.
#--------------------------------------------------------------------------------------------

set(CMAKE_CONFIGURATION_TYPES
    "Debug;Develop;Release;Performance"
    CACHE STRING
    "Supported build configurations"
    FORCE
)

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE
        Develop
        CACHE STRING
        "Build type"
        FORCE
    )
endif()

set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS
    Debug
    Develop
    Release
    Performance
)