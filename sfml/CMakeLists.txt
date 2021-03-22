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

# include the configuration file
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/Config.cmake)

# We use the paths from the cmake GUNInstallDirs module as defaults.
# You can override these if you like.
include(GNUInstallDirs)

# setup version numbers
set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

# Add an option for choosing the build type (shared or static)
sfml_set_option(BUILD_SHARED_LIBS TRUE BOOL
        "TRUE to build SFML as shared libraries, FALSE to build it as static libraries")

sfml_set_option(SFML_BUILD_EXAMPLES TRUE BOOL
        "TRUE to build the SFML examples, FALSE to ignore them")

# Add options to select which modules to build.
sfml_set_option(SFML_BUILD_WINDOW FALSE BOOL
        "TRUE to build SFML's Window module. This setting is ignored, if the graphics module is build")
sfml_set_option(SFML_BUILD_GRAPHICS FALSE BOOL "TRUE to build SFML's Graphics module")
sfml_set_option(SFML_BUILD_AUDIO TRUE BOOL "TRUE to build SFML's Audio module")
sfml_set_option(SFML_BUILD_NETWORK FALSE BOOL "TRUE to build SFML's Network module")

# Force building sfml-window, if sfml-graphics module is built.
if (SFML_BUILD_GRAPHICS AND NOT SFML_BUILD_WINDOW)
    message(WARNING "You're trying to build SFML's Graphics module without the Window module.
Forcing building of the Window module as a dependency")
endif ()

# enable project folders
set_property(GLOBAL PROPERTY USE_FOLDERS ON)
set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")

# add the subdirectories
add_subdirectory(src)

if (SFML_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif ()
