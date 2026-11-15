# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
project(example LANGUAGES CXX)
configure_file(example.toml example.toml)
configure_file(example.json example.json)
configure_file(example.yaml example.yaml)
add_executable(
    example
    main.cpp
)
target_link_libraries(
    example KDSpdSetup::KDSpdSetup
)
