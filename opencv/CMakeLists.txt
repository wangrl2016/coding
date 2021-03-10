cmake_minimum_required(VERSION 3.15)

project(OpenCVTutorials)

find_package(OpenCV REQUIRED)
include_directories(${OpenCV_INCLUDE_DIRS})

add_executable(DisplayImage DisplayImage.cpp)
target_link_libraries(DisplayImage ${OpenCV_LIBS})

add_executable(ImageEnhance ImageEnhance.cpp)
target_link_libraries(ImageEnhance ${OpenCV_LIBS})

add_executable(DrawContours DrawContours.cpp)
target_link_libraries(DrawContours ${OpenCV_LIBS})

add_executable(BlendMode BlendMode)
target_link_libraries(BlendMode ${OpenCV_LIBS})
