/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include <KDSpdSetup/kdspdsetup.h>

int main()
{
    KDSPDSetup::setupFrom("../examples/example.toml");

    for (std::string &&name : { "root", "console", "global_async", "local_async" }) {
        auto logger = spdlog::get(name);
        auto message = fmt::format("hello {}", name);
        logger->debug(message);
        logger->critical(message);
        logger->error(message);
        logger->info(message);
        logger->trace(message);
        logger->warn(message);
    }
}
