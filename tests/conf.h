/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#pragma once

#include <KDSpdSetup/kdspdsetup.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

void setup(std::string &&str)
{
    KDSPDSetup::setupFrom(str);
}
