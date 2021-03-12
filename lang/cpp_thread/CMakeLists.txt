cmake_minimum_required(VERSION 3.15)

project(CppThread)

set(EXAMPLES
    HelloConcurrent
        )

foreach (EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE} pthread)
endforeach ()
