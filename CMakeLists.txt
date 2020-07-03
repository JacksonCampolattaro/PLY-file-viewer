cmake_minimum_required(VERSION 3.10)
project(PLY-file-viewer)
set(CMAKE_CXX_STANDARD 14)

set(SOURCE_FILES main.cpp)

find_package(CGAL)

add_executable(viewer ${SOURCE_FILES})

target_link_libraries(viewer CGAL::CGAL)