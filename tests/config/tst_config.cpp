/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "../conf.h"
#include <doctest/doctest.h>

using namespace KDSPDSetup;

namespace {

/// @brief Check a configuration that is expected to describe `example.toml`, `example.json`
/// and `example.yaml`, which are all the same document in different formats.
void checkExample(Configuration const &config)
{
    REQUIRE(config.global_pattern.has_value());
    CHECK(*config.global_pattern == "[%l] %v");

    REQUIRE(config.sink.size() == 2);

    CHECK(config.sink[0].name == "console");
    CHECK(config.sink[0].type == "stdout_sink_mt");
    REQUIRE(config.sink[0].level.has_value());
    CHECK(*config.sink[0].level == "debug");
    CHECK(!config.sink[0].truncate);

    CHECK(config.sink[1].name == "rotate");
    CHECK(config.sink[1].type == "rotating_file_sink_mt");
    REQUIRE(config.sink[1].base_filename.has_value());
    CHECK(*config.sink[1].base_filename == "log/rotate.log");
    REQUIRE(config.sink[1].max_size.has_value());
    CHECK(*config.sink[1].max_size == "1M");
    CHECK(config.sink[1].max_files == 3);
    CHECK(config.sink[1].truncate);

    REQUIRE(config.pattern.size() == 1);
    CHECK(config.pattern[0].name == "succient");
    CHECK(config.pattern[0].value == "succient");

    REQUIRE(config.global_thread_pool.has_value());
    CHECK(config.global_thread_pool->queue_size == 8192);
    CHECK(config.global_thread_pool->num_threads == 1);

    REQUIRE(config.thread_pool.size() == 1);
    CHECK(config.thread_pool[0].name == "tp");
    CHECK(config.thread_pool[0].queue_size == 4096);
    CHECK(config.thread_pool[0].num_threads == 2);

    REQUIRE(config.logger.size() == 2);

    CHECK(config.logger[0].name == "root");
    CHECK(config.logger[0].type.empty());
    REQUIRE(config.logger[0].sinks.size() == 1);
    CHECK(config.logger[0].sinks[0] == "console");
    REQUIRE(config.logger[0].level.has_value());
    CHECK(*config.logger[0].level == "info");
    CHECK(!config.logger[0].pattern.has_value());

    CHECK(config.logger[1].name == "async_logger");
    CHECK(config.logger[1].type == "async");
    REQUIRE(config.logger[1].sinks.size() == 1);
    CHECK(config.logger[1].sinks[0] == "rotate");
    REQUIRE(config.logger[1].pattern.has_value());
    CHECK(*config.logger[1].pattern == "succient");
    REQUIRE(config.logger[1].thread_pool.has_value());
    CHECK(*config.logger[1].thread_pool == "tp");
    REQUIRE(config.logger[1].overflow_policy.has_value());
    CHECK(*config.logger[1].overflow_policy == "overrun_oldest");
}

} // namespace

TEST_SUITE("Configuration")
{
    TEST_CASE("The same document yields the same configuration in every format.")
    {
        SUBCASE("TOML")
        {
            checkExample(configuration::load("example.toml"));
        }

        SUBCASE("JSON")
        {
            checkExample(configuration::load("example.json"));
        }

        SUBCASE("YAML")
        {
            checkExample(configuration::load("example.yaml"));
        }
    }

    TEST_CASE("An explicit format overrides the file name extension.")
    {
        checkExample(configuration::load("example.toml", configuration::Format::toml));
    }

    TEST_CASE("Documents can be parsed from memory.")
    {
        auto const config = configuration::loadFromString(R"(
[[sink]]
name = "console"
type = "stdout_sink_mt"
)",
                                                          configuration::Format::toml);

        REQUIRE(config.sink.size() == 1);
        CHECK(config.sink[0].name == "console");
    }

    TEST_CASE("Fields a document leaves out keep their defaults.")
    {
        auto const config = configuration::loadFromString(R"(
[[sink]]
name = "console"
type = "stdout_sink_mt"
)",
                                                          configuration::Format::toml);

        REQUIRE(config.sink.size() == 1);
        CHECK(!config.sink[0].level.has_value());
        CHECK(!config.sink[0].filename.has_value());
        CHECK(!config.sink[0].truncate);
        CHECK(!config.sink[0].max_files);
        CHECK(!config.sink[0].rotation_hour);
        CHECK(!config.sink[0].rotation_minute);
        CHECK(!config.sink[0].syslog_option.has_value());
        CHECK(!config.sink[0].syslog_facility.has_value());

        CHECK(!config.global_pattern.has_value());
        CHECK(!config.global_thread_pool.has_value());
        CHECK(config.pattern.empty());
        CHECK(config.thread_pool.empty());
        CHECK(config.logger.empty());
    }

    TEST_CASE("A configuration can be applied end to end.")
    {
        setupFromString(R"(
[[sink]]
name = "console"
type = "stdout_sink_mt"

[[logger]]
name = "config_e2e_root"
sinks = ["console"]
level = "info"
)",
                        configuration::Format::toml);

        auto const root = spdlog::get("config_e2e_root");
        REQUIRE(root != nullptr);
        CHECK(root->level() == spdlog::level::info);

        spdlog::drop("config_e2e_root");
    }
}
