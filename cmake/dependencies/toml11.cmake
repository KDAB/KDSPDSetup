# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
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

    fetchcontent_declare(
        toml11
        GIT_REPOSITORY https://github.com/ToruNiina/toml11.git
        GIT_TAG master
    )
    fetchcontent_makeavailable(toml11)

    set_target_properties(
        toml11
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()
