cmake_minimum_required(VERSION 3.5)

project(kdspdsetup LANGUAGES CXX
                   VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_EXAMPLES "Build the example project" ON)
option(BUILD_STATIC "Build as a static library" OFF)

find_package(spdlog)
find_package(fmt)
add_subdirectory(lib/toml11)

set(LINKLIBS spdlog fmt toml11)
if (BUILD_TESTS)
    add_subdirectory(lib/doctest)
    list(APPEND LINKLIBS doctest)
endif()

include_directories(kdspdsetup include)
if (BUILD_STATIC)
    add_library(kdspdsetup STATIC src/kdspdsetup.cpp src/details.cpp src/setup.cpp)
else()
    add_library(kdspdsetup SHARED src/kdspdsetup.cpp src/details.cpp src/setup.cpp)
endif()

target_link_libraries(kdspdsetup
    ${LINKLIBS}
)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()
