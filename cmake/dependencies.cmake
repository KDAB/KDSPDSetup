include(FetchContent)

# Note: FetchContent_MakeAvailable builds the project if it contains a
# CMakeLists.txt, otherwise it does nothing. ${package_SOURCE_DIR}
# ${package_BINARY_DIR} are made available by MakeAvailable or Populate
message(STATUS "Checking/updating dependencies. This may take a little while...
    Set the FETCHCONTENT_QUIET option to OFF to get verbose output.
")

include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies/toml11.cmake)
include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies/spdlog.cmake)

if(KDSPDSETUP_BUILD_TESTS)
  # doctest
  include(${CMAKE_CURRENT_SOURCE_DIR}/cmake/dependencies/doctest.cmake)
endif()
