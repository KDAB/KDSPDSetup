# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group
# company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
project(test-pre LANGUAGES CXX)

configure_file(pre.toml pre.toml)
add_kdspdsetup_test(${PROJECT_NAME} tst_pre.cpp)
