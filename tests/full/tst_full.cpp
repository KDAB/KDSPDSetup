#include <kdspdsetup.h>

#include "../conf.h"
#include <doctest/doctest.h>

TEST_SUITE("Tests for full.toml")
{
    void setup()
    {
        KDSPDSetup::KDSPDSetup::setup_from("full.toml");
    }

    TEST_CASE("Setup")
    {
        SUBCASE("No loggers should be created yet.")
        {
            CHECK(spdlog::get("root") == nullptr);
            CHECK(spdlog::get("console") == nullptr);
            CHECK(spdlog::get("global_async") == nullptr);
            CHECK(spdlog::get("local_async") == nullptr);
        }

        setup();

        SUBCASE("All loggers should be created.")
        {
            CHECK(spdlog::get("root") != nullptr);
            CHECK(spdlog::get("console") != nullptr);
            CHECK(spdlog::get("global_async") != nullptr);
            CHECK(spdlog::get("local_async") != nullptr);
        }
    }

    TEST_CASE("Root Logger")
    {
        SUBCASE("Root logger should still exist.")
        {
            CHECK(spdlog::get("root") != nullptr);
        }

        auto logger = spdlog::get("root");

        SUBCASE("Root logger should have 12 sinks on Linux and 14 on Windows.")
        {
#ifdef __linux__
            CHECK(logger->sinks().size() == 12);
#elif _WIN32
            CHECK(logger->sinks().size() == 14);
#endif
        }

        SUBCASE("Root logger's level should be trace.")
        {
            CHECK(logger->level() == spdlog::level::level_enum::trace);
        }
    }

    TEST_CASE("Console Logger")
    {
        SUBCASE("Console logger should still exist.")
        {
            CHECK(spdlog::get("console") != nullptr);
        }

        auto logger_console = spdlog::get("console");

        SUBCASE("Console logger should have two sinks.")
        {
            CHECK(logger_console->sinks().size() == 2);
        }
    }

    TEST_CASE("Global async logger")
    {
        SUBCASE("Global async logger should still exist.")
        {
            CHECK(spdlog::get("global_async") != nullptr);
        }

        auto logger_global_async = spdlog::get("global_async");

        SUBCASE("Global async logger should have only one sink.")
        {
            CHECK(logger_global_async->sinks().size() == 1);
        }
    }

    TEST_CASE("Local async logger")
    {
        SUBCASE("Local async logger should still exist.")
        {
            CHECK(spdlog::get("local_async") != nullptr);
        }

        auto logger_local_async = spdlog::get("local_async");

        SUBCASE("Global async logger should have only one sink.")
        {
            CHECK(logger_local_async->sinks().size() == 1);
        }
    }
}
