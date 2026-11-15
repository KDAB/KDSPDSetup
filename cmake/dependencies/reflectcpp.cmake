# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
find_package(reflectcpp QUIET)

if(NOT TARGET reflectcpp)
    get_property(tmp GLOBAL PROPERTY PACKAGES_NOT_FOUND)
    list(
        FILTER
        tmp
        EXCLUDE
        REGEX
        reflectcpp
    )
    set_property(GLOBAL PROPERTY PACKAGES_NOT_FOUND ${tmp})

    # Only the serialization formats this library can actually read are enabled; JSON is
    # always built. The remaining reflect-cpp formats (Avro, BSON, Cap'n Proto, CBOR,
    # flexbuffers, msgpack, UBJSON, XML) are not needed here and would only slow the
    # build down. These are plain CACHE variables, so an explicit -D on the command line
    # still wins.
    set(REFLECTCPP_TOML ON CACHE BOOL "Read TOML configuration files")
    set(REFLECTCPP_YAML ON CACHE BOOL "Read YAML configuration files")
    set(REFLECTCPP_AVRO OFF CACHE BOOL "Avro support is not needed")
    set(REFLECTCPP_BSON OFF CACHE BOOL "BSON support is not needed")
    set(REFLECTCPP_CAPNPROTO OFF CACHE BOOL "Cap'n Proto support is not needed")
    set(REFLECTCPP_CBOR OFF CACHE BOOL "CBOR support is not needed")
    set(REFLECTCPP_FLEXBUFFERS OFF CACHE BOOL "flexbuffers support is not needed")
    set(REFLECTCPP_MSGPACK OFF CACHE BOOL "msgpack support is not needed")
    set(REFLECTCPP_UBJSON OFF CACHE BOOL "UBJSON support is not needed")
    set(REFLECTCPP_XML OFF CACHE BOOL "XML support is not needed")

    # Note: no release tag is pinned yet, mirroring how toml11 was handled before.
    # Pin this to a reflect-cpp release tag once the integration has settled.
    fetchcontent_declare(
        reflect-cpp
        GIT_REPOSITORY https://github.com/munich-data-engineering/reflect-cpp.git
        GIT_TAG main
        EXCLUDE_FROM_ALL
        SYSTEM
    )
    fetchcontent_makeavailable(reflect-cpp)
endif()
