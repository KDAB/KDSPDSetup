# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group
# company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
project(test-full LANGUAGES CXX)

configure_file(full-win32.toml full-win32.toml)
configure_file(full-linux.toml full-linux.toml)
add_kdspdsetup_test(${PROJECT_NAME} tst_full.cpp)
