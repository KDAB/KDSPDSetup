/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "../conf.h"
#include <KDSpdSetup/configuration.h>
#include <KDSpdSetup/details.h>
#include <KDSpdSetup/setup.h>
#include <doctest/doctest.h>

#include <memory>

using namespace KDSPDSetup;
using namespace std::literals::string_literals;

namespace {

/// @brief Parse a TOML document that is expected to be rejected.
void loadToml(std::string_view content)
{
    configuration::loadFromString(content, configuration::Format::toml);
}

/// @brief Register a sink under `name`, so loggers can refer to it.
void registerStdoutSink(std::string name)
{
    details::SPDMaps::emplaceSinkMap(std::make_pair(std::move(name), std::make_shared<spdlog::sinks::stdout_sink_st>()));
}

} // namespace

TEST_SUITE("Configuration documents")
{
    TEST_CASE("The format is derived from the file name extension.")
    {
        CHECK(configuration::formatFromExtension("config.toml") == configuration::Format::toml);
        CHECK(configuration::formatFromExtension("config.json") == configuration::Format::json);
        CHECK(configuration::formatFromExtension("config.yaml") == configuration::Format::yaml);
        CHECK(configuration::formatFromExtension("config.yml") == configuration::Format::yaml);
        CHECK(configuration::formatFromExtension("config.TOML") == configuration::Format::toml);
        CHECK(configuration::formatFromExtension("path/to/config.json") == configuration::Format::json);

        CHECK(!configuration::formatFromExtension("config").has_value());
        CHECK(!configuration::formatFromExtension("config.ini").has_value());
    }

    TEST_CASE("Unknown extensions are rejected.")
    {
        CHECK_THROWS_AS(setupFrom("config.ini"), std::invalid_argument);
    }

    TEST_CASE("Missing files are rejected.")
    {
        CHECK_THROWS(setupFrom("this-file-does-not-exist.toml"));
    }

    TEST_CASE("Documents with broken syntax are rejected.")
    {
        SUBCASE("TOML")
        {
            CHECK_THROWS(loadToml("[[sink]"));
        }

        SUBCASE("JSON")
        {
            CHECK_THROWS(configuration::loadFromString("{ \"sink\": ", configuration::Format::json));
        }

        SUBCASE("YAML")
        {
            CHECK_THROWS(configuration::loadFromString("sink:\n  - name: [", configuration::Format::yaml));
        }
    }

    TEST_CASE("Documents with wrongly typed values are rejected.")
    {
        SUBCASE("A level must be a string")
        {
            CHECK_THROWS(loadToml(R"(
[[logger]]
name = "invalid"
level = 1
)"));
        }

        SUBCASE("Sinks must be an array of strings")
        {
            CHECK_THROWS(loadToml(R"(
[[logger]]
name = "invalid"
sinks = "console"
)"));
        }

        SUBCASE("A sink must be a table")
        {
            CHECK_THROWS(loadToml("sink = [1, 2, 3]"));
        }

        SUBCASE("Rotation hours must be numbers")
        {
            CHECK_THROWS(loadToml(R"(
[[sink]]
name = "daily"
type = "daily_file_sink_st"
base_filename = "log/daily.log"
rotation_hour = "seventeen"
)"));
        }
    }

    TEST_CASE("Incomplete documents are rejected.")
    {
        SUBCASE("A sink requires a name")
        {
            CHECK_THROWS(loadToml(R"(
[[sink]]
type = "stdout_sink_mt"
)"));
        }

        SUBCASE("A sink requires a type")
        {
            CHECK_THROWS(loadToml(R"(
[[sink]]
name = "console"
)"));
        }

        SUBCASE("A basic file sink requires a filename")
        {
            CHECK_THROWS(loadToml(R"(
[[sink]]
name = "file"
type = "basic_file_sink_st"
)"));
        }

        SUBCASE("A rotating file sink requires a max size")
        {
            CHECK_THROWS(loadToml(R"(
[[sink]]
name = "rotate"
type = "rotating_file_sink_st"
base_filename = "log/rotate.log"
)"));
        }

        SUBCASE("A logger requires a name")
        {
            CHECK_THROWS(loadToml(R"(
[[logger]]
sinks = ["console"]
)"));
        }

        SUBCASE("A pattern requires a value")
        {
            CHECK_THROWS(loadToml(R"(
[[pattern]]
name = "succient"
)"));
        }
    }
}

TEST_SUITE("Setup functions")
{
    TEST_CASE("Sinks with an unknown type are rejected.")
    {
        CHECK_THROWS_AS(setup::setupSink(SinkConfig{ .name = "unknown", .type = "no_such_sink" }), std::out_of_range);
    }

    TEST_CASE("Sinks with an unknown level are rejected.")
    {
        auto const sink = SinkConfig{ .name = "invalid_level", .type = "stdout_sink_mt", .level = "no_such_level" };

        CHECK_THROWS_AS(setup::setupSink(sink), std::out_of_range);
    }

    TEST_CASE("Loggers with an unknown level are rejected.")
    {
        registerStdoutSink("console_for_level");

        auto const logger = LoggerConfig{ .name = "invalid_level", .sinks = { "console_for_level" }, .level = "no_such_level" };

        CHECK_THROWS_AS(setup::setupLogger(logger), std::out_of_range);
    }

    TEST_CASE("Loggers referring to an unknown pattern are rejected.")
    {
        registerStdoutSink("console_for_pattern");

        auto const logger = LoggerConfig{ .name = "invalid_pattern", .sinks = { "console_for_pattern" }, .pattern = "no_such_pattern" };

        CHECK_THROWS_AS(setup::setupLogger(logger), std::out_of_range);
    }

    TEST_CASE("Loggers referring to an unknown thread pool are rejected.")
    {
        registerStdoutSink("console_for_pool");

        auto const logger = LoggerConfig{ .name = "invalid_pool",
                                          .type = "async",
                                          .sinks = { "console_for_pool" },
                                          .thread_pool = "no_such_pool" };

        CHECK_THROWS_AS(setup::setupLogger(logger), std::out_of_range);
    }

    TEST_CASE("Loggers with an unknown overflow policy are rejected.")
    {
        registerStdoutSink("console_for_overflow");

        auto const logger = LoggerConfig{ .name = "invalid_overflow",
                                          .type = "async",
                                          .sinks = { "console_for_overflow" },
                                          .overflow_policy = "no_such_policy" };

        CHECK_THROWS_AS(setup::setupLogger(logger), std::out_of_range);
    }

    TEST_CASE("Loggers silently skip sinks that were never set up.")
    {
        auto const logger = LoggerConfig{ .name = "missing_sink", .sinks = { "never_registered" } };

        CHECK_NOTHROW(setup::setupLogger(logger));

        spdlog::drop("missing_sink");
    }

    TEST_CASE("Loggers are created from the configuration.")
    {
        registerStdoutSink("console_for_creation");

        auto const logger = LoggerConfig{ .name = "created_logger", .sinks = { "console_for_creation" }, .level = "info" };

        CHECK(spdlog::get("created_logger") == nullptr);

        setup::setupLogger(logger);

        auto const created = spdlog::get("created_logger");
        REQUIRE(created != nullptr);
        CHECK(created->level() == spdlog::level::info);
        CHECK(created->sinks().size() == 1);

        spdlog::drop("created_logger");
    }
}
