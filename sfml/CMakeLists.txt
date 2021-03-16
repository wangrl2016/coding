cmake_minimum_required(VERSION 3.15)

# define a macro that helps defining an option
macro(SFML_SET_OPTION val default type docstring)
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
SFML_SET_OPTION(CMAKE_BUILD_TYPE Release STRING "Choose the type of build (Debug or Release)")

# project name
project(SFML)

if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(SFML_OS_UNIX 1)
    set(SFML_OS_LINUX 1)
endif ()

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

SFML_SET_OPTION(BUILD_SHARED_LIBS TRUE BOOL "TRUE to build SFML as shared libraries")
SFML_SET_OPTION(SFML_BUILD_AUDIO TRUE BOOL "TRUE to build SFML's Audio module")

# add the subdirectories
add_subdirectory(src)