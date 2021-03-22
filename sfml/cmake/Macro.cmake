
# Add a new target which is a SFML library.
# example: sfml_add_library(sfml-graphics
#           SOURCES sprite.cpp image.cpp ...
#           [STATIC]) # Always create a static library and ignore BUILD_SHARED_LIBS
macro(sfml_add_library target)
    # parse the arguments
    cmake_parse_arguments(THIS "STATIC" "" "SOURCES" ${ARGN})
    if (NOT "${THIS_UNPARSED_ARGUMENTS}" STREQUAL "")
        message(FATAL_ERROR "Extra unparsed arguments when calling sfml_add_library: ${THIS_UNPARSED_ARGUMENTS}")
    endif ()

    # Create the target.
    if (THIS_STATIC)
        add_library(${target} STATIC ${THIS_SOURCES})
    else ()
        add_library(${target} ${THIS_SOURCES})
    endif ()
endmacro()