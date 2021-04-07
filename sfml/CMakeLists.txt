cmake_minimum_required(VERSION 3.15)

# define a macro that helps defining an option
macro(sfml_set_option val default type docstring)
    if (NOT DEFINE ${var})
        set(${var} ${default})
    endif ()
    # set Set a CMake, cache or environment variable to a given value
    # set(<variable> <value> [[CACHE <type> <docstring> [FORCE]] | PARENT_SCOPE)
    set(${var} ${${var}} CACHE ${type} ${docstring} FORCE)
endmacro()

# These options have to be set before CMake detects/configures the toolchain
# Determine whether to create a debug or release build.
#
# CMAKE_BUILD_TYPE specifies what build type (configuration) will be built in this build tree.
# Possibly values are empty,
# Debug, Release, RelWithDebInfo, MinSizeRel,...
# -DCMAKE_BUILD_TYPE=Release
sfml_set_option(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# project name
project(SFML)

# We use the paths from the cmake GUNInstallDirs module as defaults.
# You can override these if you like.
include(GNUInstallDirs)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

include_directories(${CMAKE_SOURCE_DIR})

add_library(audio src/audio/AlResource.cpp)

add_executable(sound Sound.cpp)
target_link_libraries(sound audio)
