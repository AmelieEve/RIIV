cmake_minimum_required(VERSION 3.24)
project(riiv)

set(CMAKE_CXX_STANDARD 17)
find_package(OpenCV REQUIRED)

include_directories(include ${OpenCV_INCLUDE_DIRS})

add_executable(riiv)

target_link_libraries(riiv ${OpenCV_LIBS})