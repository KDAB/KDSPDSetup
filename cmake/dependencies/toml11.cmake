find_package(toml11 QUIET)

if(NOT TARGET toml11)
    get_property(tmp GLOBAL PROPERTY PACKAGES_NOT_FOUND)
    list(
        FILTER
        tmp
        EXCLUDE
        REGEX
        toml11
    )
    set_property(GLOBAL PROPERTY PACKAGES_NOT_FOUND ${tmp})

    FetchContent_Declare(
        toml11
        GIT_REPOSITORY https://github.com/ToruNiina/toml11.git
        GIT_TAG master
    )
    FetchContent_MakeAvailable(toml11)

    set_target_properties(
        toml11
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()
