cmake_minimum_required(VERSION 3.15)

project(OpenCVTutorials)

add_compile_options(-fPIC)
add_compile_options(-W)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

set(EXAMPLES
        DisplayImage
        ImageEnhance
        DrawContours
        BlendMode
        )

foreach (EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp)
    target_link_libraries(${EXAMPLE}  ${OpenCV_LIBS})
endforeach ()
