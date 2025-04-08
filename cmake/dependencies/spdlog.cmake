# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
find_package(spdlog 1.11 QUIET)

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

    # Fix for uninitialized variable
    set(PKG_CONFIG_REQUIRES "" CACHE STRING "Pkg-config requires field")

    fetchcontent_declare(
        spdlog
        GIT_REPOSITORY https://github.com/gabime/spdlog.git
        GIT_TAG v1.11.0
        EXCLUDE_FROM_ALL
        SYSTEM
    )
    fetchcontent_makeavailable(spdlog)

    set_target_properties(
        spdlog
        PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib"
                   RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin"
    )
endif()
