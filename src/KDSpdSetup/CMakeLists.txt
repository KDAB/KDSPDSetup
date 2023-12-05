# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group
# company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
set(HEADERS details.h kdspdsetup.h setup.h)

set(SOURCES details.cpp kdspdsetup.cpp setup.cpp)

add_library(KDSpdSetup STATIC ${SOURCES} ${HEADERS})
add_library(KDSpdSetup::KDSpdSetup ALIAS KDSpdSetup)

set(KDSPDSETUP_PUBLIC_LIBS spdlog::spdlog toml11::toml11)

set(KDSPDSETUP_EXPORT_TARGETS KDSpdSetup)

target_link_libraries(KDSpdSetup PUBLIC ${KDSPDSETUP_PUBLIC_LIBS})

# Require >=C++20 for us and downstream projects
target_compile_features(KDSpdSetup PUBLIC cxx_std_20)

target_include_directories(
  KDSpdSetup
  PUBLIC $<BUILD_INTERFACE:${CMAKE_BINARY_DIR}/include>
         $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
         $<INSTALL_INTERFACE:include>)

set_target_properties(
  KDSpdSetup
  PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
             RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
             CXX_VISIBILITY_PRESET hidden
             VISIBILITY_INLINES_HIDDEN 1)

include(GenerateExportHeader)
generate_export_header(KDSpdSetup BASE_NAME kdspdsetup)
configure_file(${CMAKE_CURRENT_BINARY_DIR}/kdspdsetup_export.h
               ${CMAKE_BINARY_DIR}/include/KDSpdSetup/kdspdsetup_export.h)
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/kdspdsetup_export.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/KDSpdSetup)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/config.h
               ${CMAKE_BINARY_DIR}/include/KDSpdSetup/config.h)
install(FILES ${CMAKE_BINARY_DIR}/include/KDSpdSetup/config.h
        DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/KDSpdSetup)

add_feature_info(KDSpdSetup ON "Build Library")

foreach(file ${HEADERS})
  get_filename_component(dir ${file} DIRECTORY)
  install(FILES ${file}
          DESTINATION ${CMAKE_INSTALL_INCLUDEDIR}/KDSpdSetup/${dir})
endforeach()

install(
  TARGETS ${KDSPDSETUP_EXPORT_TARGETS}
  EXPORT KDSpdSetup
  LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
  ARCHIVE DESTINATION ${CMAKE_INSTALL_LIBDIR}
  RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR})

install(
  EXPORT KDSpdSetup
  DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/KDSpdSetup
  NAMESPACE KDSpdSetup::
  FILE KDSpdSetupTargets.cmake)

include(CMakePackageConfigHelpers)

configure_package_config_file(
  ${CMAKE_CURRENT_SOURCE_DIR}/cmake/KDSpdSetupConfig.cmake.in
  ${CMAKE_CURRENT_BINARY_DIR}/KDSpdSetupConfig.cmake
  INSTALL_DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/KDSpdSetup)

write_basic_package_version_file(
  KDSpdSetupConfigVersion.cmake
  VERSION ${PROJECT_VERSION}
  COMPATIBILITY SameMajorVersion)

install(FILES ${CMAKE_CURRENT_BINARY_DIR}/KDSpdSetupConfig.cmake
              ${CMAKE_CURRENT_BINARY_DIR}/KDSpdSetupConfigVersion.cmake
        DESTINATION ${CMAKE_INSTALL_LIBDIR}/cmake/KDSpdSetup)
