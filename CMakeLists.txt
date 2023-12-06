# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group
# company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
cmake_minimum_required(VERSION 3.5)

project(
  kdspdsetup
  LANGUAGES CXX
  VERSION 0.1)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_POSITION_INDEPENDENT_CODE ON)

option(KDSPDSETUP_BUILD_TESTS "Build unit tests" ON)
option(KDSPDSETUP_BUILD_EXAMPLES "Build the example project" ON)
option(KDSPDSETUP_BUILD_DOCS "Build Doxygen documentation" ON)
set(SPDLOG_INSTALL ON)

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
