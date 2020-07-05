
set(SOURCE_FILES main.cpp Viewer.h Camera.h)

find_package(CGAL)

find_package(Magnum REQUIRED
        GL
        MeshTools
        Primitives
        Shaders)

if(MAGNUM_TARGET_GLES AND NOT MAGNUM_TARGET_DESKTOP_GLES)
    find_package(Magnum REQUIRED EglContext)
elseif(CORRADE_TARGET_WINDOWS)
    find_package(Magnum REQUIRED WglContext)
elseif(CORRADE_TARGET_APPLE)
    find_package(Magnum REQUIRED CglContext)
elseif(CORRADE_TARGET_UNIX)
    find_package(Magnum REQUIRED GlxContext)
else()
    message(FATAL_ERROR "Magnum context creation is not supported on this platform")
endif()

#set_directory_properties(PROPERTIES CORRADE_USE_PEDANTIC_FLAGS ON)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GTKMM gtkmm-3.0)
include_directories(SYSTEM ${GTKMM_INCLUDE_DIRS})

add_executable(viewer ${SOURCE_FILES})

target_link_libraries(viewer
        CGAL::CGAL
        Magnum::GLContext
        Magnum::GL
        Magnum::Magnum
        Magnum::MeshTools
        Magnum::Primitives
        Magnum::Shaders
        ${GTKMM_LIBRARIES})
