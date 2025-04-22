cmake_minimum_required(VERSION 3.12)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(TrillionPass VERSION 1.0)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

include_directories(
    ${CMAKE_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}/dependencies/include
    )
file(GLOB_RECURSE SRC "src/*.cpp")

if (MSVC)
    add_compile_options("/utf-8")
endif()

add_executable(${PROJECT_NAME} ${SRC})
target_precompile_headers(${PROJECT_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include/pch.hpp)

if (MSVC)
    set_target_properties(${PROJECT_NAME} PROPERTIES WIN32_EXECUTABLE OFF)
endif()

set_target_properties(${PROJECT_NAME} PROPERTIES 
    RUNTIME_OUTPUT_DIRECTORY_RELEASE ${CMAKE_SOURCE_DIR}/bin/release
    RUNTIME_OUTPUT_DIRECTORY_DEBUG ${CMAKE_SOURCE_DIR}/bin/debug)

target_compile_definitions(${PROJECT_NAME} PRIVATE PROJECT_PATH="${CMAKE_SOURCE_DIR}")

if (WIN32)
    target_link_libraries(${PROJECT_NAME} bcrypt)
endif()