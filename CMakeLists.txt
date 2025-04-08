# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
cmake_minimum_required(VERSION 3.11)

project(
    kdspdsetup
    LANGUAGES CXX
    VERSION 0.1
)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(KDSPDSETUP_BUILD_TESTS "Build unit tests" OFF)
option(KDSPDSETUP_BUILD_EXAMPLES "Build the example project" ON)
option(KDSPDSETUP_BUILD_DOCS "Build Doxygen documentation" OFF)

set(SPDLOG_INSTALL ON)

message("\nCompiler: ${CMAKE_CXX_COMPILER} ${CMAKE_CXX_COMPILER_VERSION}\n")

include(FeatureSummary)
include(GNUInstallDirs)

# Note: Set before including dependencies.cmake, so 3rd party libs end up in the
# correct dir
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib")
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies.cmake)

add_subdirectory(src/KDSpdSetup)

if(KDSPDSETUP_BUILD_TESTS)
    enable_testing()
    add_subdirectory(tests)
endif()

if(KDSPDSETUP_BUILD_EXAMPLES)
    add_subdirectory(examples)
endif()

if(KDSPDSETUP_BUILD_DOCS)
    add_subdirectory(docs)
endif()

add_custom_target(cppcheck
    USES_TERMINAL
    WORKING_DIRECTORY ${PROJECT_BINARY_DIR}
    COMMAND ${CMAKE_COMMAND} -E env
    cppcheck
        "--project=${PROJECT_BINARY_DIR}/compile_commands.json"
        --enable=all
        --error-exitcode=1
        --language=c++
        --inline-suppr
        --quiet
        --disable=missingInclude,unusedFunction
        --check-level=exhaustive
        --library=qt.cfg
        "-i${CMAKE_CURRENT_BINARY_DIR}/_deps/"
        "--suppress=*:${CMAKE_CURRENT_BINARY_DIR}/_deps/"
)
