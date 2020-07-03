
set(SOURCE_FILES main.cpp)

find_package(CGAL)

add_executable(viewer ${SOURCE_FILES})

target_link_libraries(viewer CGAL::CGAL)
