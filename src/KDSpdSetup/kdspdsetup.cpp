/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "kdspdsetup.h"

#include <KDSpdSetup/setup.h>

namespace KDSPDSetup {

namespace {

/// @brief Apply a parsed configuration to spdlog.
void apply(Configuration const &config)
{
    setup::setupSinks(config);
    setup::setupPatterns(config);
    setup::setupThreadPools(config);
    setup::setupLoggers(config);
}

} // namespace

void setupFrom(std::string const &file)
{
    apply(configuration::load(file));
}

void setupFrom(std::string const &file, configuration::Format format)
{
    apply(configuration::load(file, format));
}

void setupFromString(std::string_view content, configuration::Format format)
{
    apply(configuration::loadFromString(content, format));
}

} // namespace KDSPDSetup
