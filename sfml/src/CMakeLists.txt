#include the SFML specific macros
include(${PROJECT_SOURCE_DIR}/cmake/Macro.cmake)


# sfml-system
add_subdirectory(system)

# sfml-window
if (SFML_BUILD_WINDOW OR SFML_BUILD_GRAPHICS)
    add_subdirectory(window)
endif ()

# sfml-network
if (SFML_BUILD_NETWORK)
    add_subdirectory(network)
endif ()

# sfml-graphics
if (SFML_BUILD_GRAPHICS)
    add_subdirectory(graphics)
endif ()

# sfml-audio
if (SFML_BUILD_AUDIO)
    message("hello")
    add_subdirectory(audio)
endif ()

sfml_export_targets()