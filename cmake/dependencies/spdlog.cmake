find_package(spdlog QUIET)
find_package(fmt QUIET)

if(NOT TARGET spdlog::spdlog)
    get_property(tmp GLOBAL PROPERTY PACKAGES_NOT_FOUND)
    list(
        FILTER
        tmp
        EXCLUDE
        REGEX
        spdlog
    )
    set_property(GLOBAL PROPERTY PACKAGES_NOT_FOUND ${tmp})

    FetchContent_Declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
    )
    FetchContent_MakeAvailable(spdlog)

    set_target_properties(
        spdlog
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()

if(NOT TARGET fmt::fmt)
    get_property(tmp GLOBAL PROPERTY PACKAGES_NOT_FOUND)
    list(
        FILTER
        tmp
        EXCLUDE
        REGEX
        fmt
    )
    set_property(GLOBAL PROPERTY PACKAGES_NOT_FOUND ${tmp})

    FetchContent_Declare(fmt
        GIT_REPOSITORY https://github.com/fmtlib/fmt.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(fmt)

    set_target_properties(
        fmt
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()
