/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "kdspdsetup.h"

#include <KDSpdSetup/setup.h>

namespace KDSPDSetup {

void setupFrom(std::string const &file)
{
    auto const data = toml::parse(file);

    setup::setupPatterns(data);
    setup::setupSinks(data);
    setup::setupThreadPools(data);
    setup::setupLoggers(data);
}

} // namespace KDSPDSetup
