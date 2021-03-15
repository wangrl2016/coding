cmake_minimum_required(VERSION 3.15)

project(OpenGLTutorials)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

include_directories(glm)

set(EXAMPLES
        HelloWindow
        HelloTriangle
        HelloRectangle
        TwoTriangle
        ColorTriangle
        DrawCircle
        HelloTexture
        HelloTransform
        DrawSphere
        CoordinateSystem
        CameraView
        LightSpecular
        )

foreach (EXAMPLE ${EXAMPLES})
    add_executable(${EXAMPLE} ${EXAMPLE}.cpp
            glad/glad.c
            Sphere.cpp
            Cuboid.cpp
            Utils.cpp)
    target_link_libraries(${EXAMPLE} ${OPENGL_LIBRARIES} glfw dl)
endforeach ()


