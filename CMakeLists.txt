cmake_minimum_required(VERSION 3.5)

project(kdspdsetup LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(kdspdsetup main.cpp)

include(GNUInstallDirs)
install(TARGETS kdspdsetup
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)
