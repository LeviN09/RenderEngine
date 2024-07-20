cmake_minimum_required(VERSION 3.30.0)

project(RenderEngine VERSION 0.0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_EXTENSIONS False)

set(CMAKE_C_COMPILER $ENV{CC})
set(CMAKE_CXX_COMPILER $ENV{CXX})

# Includes
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/src)
include_directories(${PROJECT_SOURCE_DIR}/vendor/someLibrary/include)

# Libraries
find_package(OpenGL REQUIRED)
find_package(glm REQUIRED)
find_package(PkgConfig REQUIRED)
pkg_search_module(GLFW REQUIRED glfw3)
find_package(GLEW REQUIRED)

include_directories(${OPENGL_INCLUDE_DIR})
include_directories(${GLFW_INCLUDE_DIRS})
include_directories(${GLEW_INCLUDE_DIRS})

file(GLOB_RECURSE SOURCES "src/*.cpp")

add_library(stb_image STATIC ${PROJECT_SOURCE_DIR}/vendor/src/stb_image.cpp)
target_include_directories(stb_image PUBLIC ${PROJECT_SOURCE_DIR}/vendor/include)

add_executable(RenderEngine main.cpp ${SOURCES})

target_link_libraries(RenderEngine ${OPENGL_LIBRARIES})
target_link_libraries(RenderEngine ${GLFW_LIBRARIES})
target_link_libraries(RenderEngine ${GLEW_LIBRARIES})
target_link_libraries(RenderEngine glm::glm)
target_link_libraries(RenderEngine GLU)
target_link_libraries(RenderEngine glut)
target_link_libraries(RenderEngine m)

target_link_libraries(RenderEngine stb_image)