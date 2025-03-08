cmake_minimum_required(VERSION 3.31)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED True)

project(EasyWin32)
include_directories(${CMAKE_SOURCE_DIR}/include)
include_directories(${CMAKE_SOURCE_DIR}/include/${PROJECT_NAME})


file(GLOB_RECURSE CPP_SOURCES "src/*.cpp")

add_compile_options(/utf-8)
if (MSVC)
    add_compile_options(/Zc:preprocessor)
endif()
add_executable(${PROJECT_NAME} WIN32 ${CPP_SOURCES})

