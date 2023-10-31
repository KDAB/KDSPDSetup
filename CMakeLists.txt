cmake_minimum_required(VERSION 3.5)

project(kdspdsetup LANGUAGES CXX
                   VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_EXAMPLES "Build the example project" ON)

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

set(LINKLIBS spdlog fmt toml11)
if (BUILD_TESTS)
    list(APPEND LINKLIBS doctest)
endif()

include_directories(kdspdsetup include)
add_library(kdspdsetup STATIC src/kdspdsetup.cpp src/details.cpp src/setup.cpp)

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
