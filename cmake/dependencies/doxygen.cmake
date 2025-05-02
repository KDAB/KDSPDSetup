# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
find_package(Doxygen QUIET)

if(NOT DOXYGEN_FOUND)
    get_property(tmp GLOBAL PROPERTY PACKAGES_NOT_FOUND)
    list(
        FILTER
        tmp
        EXCLUDE
        REGEX
        Doxygen
    )
    set_property(GLOBAL PROPERTY PACKAGES_NOT_FOUND ${tmp})

    fetchcontent_declare(
        Doxygen
        GIT_REPOSITORY https://github.com/doxygen/doxygen.git
        GIT_TAG Release_1_9_5
        EXCLUDE_FROM_ALL
        SYSTEM
    )
    fetchcontent_makeavailable(Doxygen)

    set_target_properties(
        doxygen
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )

    set(DOXYGEN_EXECUTABLE ${CMAKE_BINARY_DIR}/_deps/doxygen-build/bin/doxygen)
endif()
