cmake_minimum_required(VERSION 3.17)
set(CMAKE_VERBOSE_MAKEFILE ON)

project(pass VERSION 1.0)

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

target_compile_definitions(${PROJECT_NAME} PRIVATE 
TPASS_VERSION="${PROJECT_VERSION}"
)

find_package(PkgConfig REQUIRED)
pkg_search_module(LIBSODIUM REQUIRED libsodium)

target_include_directories(${PROJECT_NAME} PRIVATE ${LIBSODIUM_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PRIVATE ${LIBSODIUM_LIBRARIES})

if (WIN32)
    target_link_libraries(${PROJECT_NAME} PRIVATE bcrypt)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Release")
    set(CMAKE_CXX_FLAGS_RELEASE "${CMAKE_CXX_FLAGS_RELEASE} -O3")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
    PROJECT_PATH="."
    )
elseif (CMAKE_BUILD_TYPE STREQUAL "Debug")
    target_compile_definitions(${PROJECT_NAME} PRIVATE 
    PROJECT_PATH="${CMAKE_SOURCE_DIR}"
    )
endif()