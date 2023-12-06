/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "../conf.h"
#include <doctest/doctest.h>

TEST_SUITE("Tests for pre.toml")
{
    TEST_CASE("")
    {
        SUBCASE("")
        {
            CHECK(spdlog::get("root") == nullptr);
        }

        setup("pre.toml");

        SUBCASE("")
        {
            CHECK(spdlog::get("root") != nullptr);
        }

        auto logger = spdlog::get("root");

        SUBCASE("")
        {
            CHECK(logger->sinks().size() == 3);
            CHECK(logger->sinks().at(0)->level() == spdlog::level::trace);
            CHECK(logger->sinks().at(1)->level() == spdlog::level::info);
            CHECK(logger->sinks().at(2)->level() == spdlog::level::err);
        }

        SUBCASE("")
        {
            CHECK(logger->level() == spdlog::level::trace);
        }
    }
}
