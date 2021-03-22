
if (${CMAKE_SYSTEM_NAME} STREQUAL "Linux")
    set(SFML_OS_UNIX 1)
    set(SFML_OS_LINUX 1)
    # don't use the OpenGL ES implementation on Linux
    set(OPENGL_ES 0)
endif ()

if (CMAKE_CXX_COMPILER_ID)
    set(SFML_COMPILER_GCC 1)
    # Runs the given sequences of one or more commands.
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpversion" OUTPUT_VARIABLE GCC_VERSION_OUTPUT)
    # /usr/bin/c++7
    string(STRIP "${GCC_VERSION_OUTPUT}" GCC_VERSION_OUTPUT)
    message(${CMAKE_CXX_COMPILER} ${GCC_VERSION_OUTPUT})
    string(REGEX REPLACE "([0-9]+\\.[0-9]+).*" "\\1" SFML_GCC_VERSION "${GCC_VERSION_OUTPUT}")
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "--version" OUTPUT_VARIABLE GCC_COMPILER_VERSION)
    string(REGEX MATCHALL ".*(tdm[64]*-[1-9]).*" SFML_COMPILER_GCC_TDM "${GCC_COMPILER_VERSION}")
    execute_process(COMMAND "${CMAKE_CXX_COMPILER}" "-dumpmachine" OUTPUT_VARIABLE GCC_MACHINE)
    string(STRIP "${GCC_MACHINE}" GCC_MACHINE)
    message(${GCC_MACHINE})

endif ()