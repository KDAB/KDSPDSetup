cmake_minimum_required(VERSION 3.5)

project(kdspdsetup LANGUAGES CXX
                   VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(BUILD_TESTS "Build unit tests" ON)
option(BUILD_EXAMPLES "Build the example project" ON)
set(SPDLOG_INSTALL ON)

include(${CMAKE_SOURCE_DIR}/cmake/dependencies.cmake)

set(LINKLIBS
    $<BUILD_INTERFACE:spdlog>
    $<BUILD_INTERFACE:toml11>
)
if (BUILD_TESTS)
list(APPEND LINKLIBS $<BUILD_INTERFACE:doctest>)
endif()

include_directories(kdspdsetup include)
add_library(kdspdsetup STATIC src/kdspdsetup.cpp src/details.cpp src/setup.cpp)

target_link_libraries(kdspdsetup
    PUBLIC
    ${LINKLIBS}
)

if (BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if (BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

include(GNUInstallDirs)

set(KDSPDSETUP_CMAKE_CONFIG_DIR "${CMAKE_INSTALL_LIBDIR}/cmake/kdspdsetup")

include(CMakePackageConfigHelpers)
configure_package_config_file(
    cmake/kdspdsetupConfig.cmake.in
    "${CMAKE_CURRENT_BINARY_DIR}/kdspdsetupConfig.cmake"
    INSTALL_DESTINATION "${KDSPDSETUP_CMAKE_CONFIG_DIR}"
)

install(
    TARGETS kdspdsetup
    EXPORT kdspdsetup
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

install(FILES "include/kdspdsetup.h" DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/kdspdsetup)

install(
    EXPORT kdspdsetup
    FILE kdspdsetup.cmake
    DESTINATION "${KDSPDSETUP_CMAKE_CONFIG_DIR}"
    NAMESPACE kdspdsetup::
)

install(
    FILES "${CMAKE_CURRENT_BINARY_DIR}/kdspdsetupConfig.cmake"
    DESTINATION "${KDSPDSETUP_CMAKE_CONFIG_DIR}"
)
