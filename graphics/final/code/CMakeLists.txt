CMAKE_MINIMUM_REQUIRED(VERSION 2.8.12)
PROJECT(final CXX)

IF(NOT CMAKE_BUILD_TYPE)
    SET(CMAKE_BUILD_TYPE Release)
ENDIF()

ADD_SUBDIRECTORY(deps/vecmath)

SET(final_SOURCES
        src/image.cpp
        src/main.cpp
        src/mesh.cpp
        src/scene_parser.cpp)

SET(final_INCLUDES
        include/bound.hpp
        include/bvh.hpp
        include/camera.hpp
        include/group.hpp
        include/hit.hpp
        include/image.hpp
        include/light.hpp
        include/material.hpp
        include/mcpt.hpp
        include/mesh.hpp
        include/object3d.hpp
        include/path_tracing.hpp
        include/plane.hpp
        include/ptutil.hpp
        include/ray.hpp
        include/scene_parser.hpp
        include/sphere.hpp
        include/transform.hpp
        include/triangle.hpp
        include/util.hpp
        include/whitted_style.hpp
        )

SET(CMAKE_CXX_STANDARD 11)

find_package(OpenMP REQUIRED)
ADD_EXECUTABLE(${PROJECT_NAME} ${final_SOURCES} ${final_INCLUDES})
target_link_libraries(${PROJECT_NAME} PRIVATE vecmath)
target_include_directories(${PROJECT_NAME} PRIVATE include)
if(OpenMP_CXX_FOUND)
        target_link_libraries(${PROJECT_NAME} PRIVATE OpenMP::OpenMP_CXX)
endif()