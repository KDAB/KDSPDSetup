cmake_minimum_required(VERSION 3.5)

project(kdspdsetup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(kdspdsetup
    examples/main.cpp
    include/kdspdsetup/kdspdsetup.h
    src/kdspdsetup.cpp)

find_package(spdlog)
find_package(fmt)

add_subdirectory("lib/toml11")

target_link_libraries(kdspdsetup toml11 spdlog fmt)

include(GNUInstallDirs)
install(TARGETS kdspdsetup
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
