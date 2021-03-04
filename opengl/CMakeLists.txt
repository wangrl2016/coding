cmake_minimum_required(VERSION 3.15)

project(OpenGLTutorials)

find_package(OpenGL REQUIRED)
find_package(glfw3 REQUIRED)

include_directories(${CMAKE_CURRENT_SOURCE_DIR})

add_executable(HelloWindow HelloWindow.cpp glad/glad.c)
target_link_libraries(HelloWindow ${OPENGL_LIBRARIES} glfw dl)

add_executable(HelloTriangle HelloTriangle.cpp glad/glad.c)
target_link_libraries(HelloTriangle ${OPENGL_LIBRARIES} glfw dl)

add_executable(HelloRectangle HelloRectangle.cpp glad/glad.c)
target_link_libraries(HelloRectangle ${OPENGL_LIBRARIES} glfw dl)

add_executable(TwoTriangle TwoTriangle.cpp glad/glad.c)
target_link_libraries(TwoTriangle ${OPENGL_LIBRARIES} glfw dl)

add_executable(ColorTriangle ColorTriangle.cpp glad/glad.c)
target_link_libraries(ColorTriangle ${OPENGL_LIBRARIES} glfw dl)