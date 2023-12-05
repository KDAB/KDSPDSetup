#include "../conf.h"
#include <KDSpdSetup/details.h>
#include <KDSpdSetup/setup.h>
#include <doctest/doctest.h>

using namespace toml::literals::toml_literals;

TEST_SUITE("Testing all functions with invalid args.")
{
    TEST_CASE("setupLogger")
    {
        CHECK(spdlog::get("") != nullptr); // weirdly this always exists?

        SUBCASE("name")
        {
            auto const value = u8R"(
                global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                [[sink]]
                name = "null_sink_st"
                type = "null_sink_st"
            )"_toml;

            KDSPDSetup::setup::setupSinks(value);
            KDSPDSetup::setup::setupPatterns(value);
            KDSPDSetup::setup::setupThreadPools(value);

            SUBCASE("name integer")
            {
                auto table = u8R"(
                    name = 12
                    sinks = ["null_sink_st"]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));

                auto i{ 0 };
                spdlog::details::registry::instance().apply_all([&i](auto log) {
                    ++i;
                    CHECK(log->name() == "");
                });
                CHECK(i == 1);
            }

            SUBCASE("name empty array")
            {
                auto table = u8R"(
                    name = []
                    sinks = ["null_sink_st"]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));

                auto i{ 0 };
                spdlog::details::registry::instance().apply_all([&i](auto log) {
                    ++i;
                    CHECK(log->name() == "");
                });
                CHECK(i == 1);
            }

            SUBCASE("name string array")
            {
                auto table = u8R"(
                    name = ["one", "two"]
                    sinks = ["null_sink_st"]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));

                auto i{ 0 };
                spdlog::details::registry::instance().apply_all([&i](auto log) {
                    ++i;
                    CHECK(log->name() == "");
                });
                CHECK(i == 1);
            }

            SUBCASE("name integer array")
            {
                auto table = u8R"(
                    name = [1, 2, 3]
                    sinks = ["null_sink_st"]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));

                auto i{ 0 };
                spdlog::details::registry::instance().apply_all([&i](auto log) {
                    ++i;
                    CHECK(log->name() == "");
                });
                CHECK(i == 1);
            }

            SUBCASE("name mixed array")
            {
                auto table = u8R"(
                    name = ["one", 2]
                    sinks = ["null_sink_st"]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));

                auto i{ 0 };
                spdlog::details::registry::instance().apply_all([&i](auto log) {
                    ++i;
                    CHECK(log->name() == "");
                });
                CHECK(i == 1);
            }
        }

        SUBCASE("sinks")
        {
            auto const value = u8R"(
                global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                [[sink]]
                name = "sinks_null_sink_st"
                type = "null_sink_st"
            )"_toml;

            KDSPDSetup::setup::setupSinks(value);
            KDSPDSetup::setup::setupPatterns(value);
            KDSPDSetup::setup::setupThreadPools(value);

            CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("sinks_null_sink_st"));

            SUBCASE("non-existent sinks")
            {
                auto table = u8R"(
                    name = "non_existent_sinks"
                    sinks = ["console_st", "daily_out"]
                )"_toml.as_table();

                CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("non_existent_sinks");
                CHECK(logger != nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 2);

                CHECK(logger->sinks().size() == 0);
                CHECK(logger->level() == spdlog::level::info); // why?
                CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
            }

            SUBCASE("some non-existent sinks")
            {
                auto table = u8R"(
                    name = "some_non_existent_sinks"
                    sinks = ["console_st", "sinks_null_sink_st"]
                )"_toml.as_table();

                CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("some_non_existent_sinks");
                CHECK(logger != nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 3);

                CHECK(logger->sinks().size() == 1);
                CHECK(logger->sinks()[0] == KDSPDSetup::details::SPDMaps::sinkMap().at("sinks_null_sink_st"));
                CHECK(logger->level() == spdlog::level::info); // why?
                CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
            }

            SUBCASE("sinks integer")
            {
                auto table = u8R"(
                    name = "sinks_integer"
                    sinks = 12
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("sinks_integer");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 3);
            }

            SUBCASE("sinks string")
            {
                auto table = u8R"(
                    name = "sinks_string"
                    sinks = "sinks_null_sink_st"
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("sinks_string");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 3);
            }

            SUBCASE("sinks empty array")
            {
                auto table = u8R"(
                    name = "sinks_empty_array"
                    sinks = []
                )"_toml.as_table();

                CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("sinks_empty_array");
                CHECK(logger != nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);

                CHECK(logger->sinks().size() == 0);
                CHECK(logger->level() == spdlog::level::info); // why?
                CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
            }

            SUBCASE("sinks integer array")
            {
                auto table = u8R"(
                    name = "sinks_integer_array"
                    sinks = [1, 2, 3]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("sinks_integer_array");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);
            }

            SUBCASE("sinks mixed array")
            {
                auto table = u8R"(
                    name = "sinks_mixed_array"
                    sinks = [1, "sinks_null_sink_st"]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("sinks_mixed_array");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);
            }
        }

        SUBCASE("level")
        {
            auto const value = u8R"(
                global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                [[sink]]
                name = "level_null_sink_mt"
                type = "null_sink_mt"
            )"_toml;

            KDSPDSetup::setup::setupSinks(value);
            KDSPDSetup::setup::setupPatterns(value);
            KDSPDSetup::setup::setupThreadPools(value);

            SUBCASE("non-existent level")
            {
                auto table = u8R"(
                    name = "non_existent_level"
                    sinks = ["level_null_sink_mt"]
                    level = "dsfsdkjfh"
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("non_existent_level");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);
            }

            SUBCASE("level integer")
            {
                auto table = u8R"(
                    name = "level_integer"
                    sinks = ["level_null_sink_mt"]
                    level = 12
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("level_integer");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);
            }

            SUBCASE("level empty array")
            {
                auto table = u8R"(
                    name = "level_empty_array"
                    sinks = ["level_null_sink_mt"]
                    level = []
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("level_empty_array");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);
            }

            SUBCASE("level integer array")
            {
                auto table = u8R"(
                    name = "level_integer_array"
                    sinks = ["level_null_sink_mt"]
                    level = [1, 2, 3]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("level_integer_array");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);
            }

            SUBCASE("level string array")
            {
                auto table = u8R"(
                    name = "level_string_array"
                    sinks = ["level_null_sink_mt"]
                    level = ["info"]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("level_string_array");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);
            }

            SUBCASE("level mixed array")
            {
                auto table = u8R"(
                    name = "level_mixed_array"
                    sinks = ["level_null_sink_mt"]
                    level = ["info", 1]
                )"_toml.as_table();

                CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                auto logger = spdlog::get("level_mixed_array");
                CHECK(logger == nullptr);

                auto i{ 0 };
                auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array" };
                spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                    ++i;
                    CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                });
                CHECK(i == 4);
            }
        }

        SUBCASE("pattern")
        {
            SUBCASE("global pattern")
            {
                auto value = u8R"(
                    [[sink]]
                    name = "global_pattern_null_sink_mt"
                    type = "null_sink_mt"
                )"_toml;

                KDSPDSetup::setup::setupSinks(value);

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("global_pattern_null_sink_mt"));

                SUBCASE("no pattern and no global pattern")
                {
                    CHECK_NOTHROW(KDSPDSetup::setup::setupPatterns(value));

                    auto table = u8R"(
                        name = "no_pattern_and_no_global_pattern"
                        sinks = ["global_pattern_null_sink_mt"]
                    )"_toml.as_table();

                    CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("no_pattern_and_no_global_pattern");
                    CHECK(logger != nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 5);

                    CHECK(logger->sinks().size() == 1);
                    CHECK(logger->sinks()[0] == KDSPDSetup::details::SPDMaps::sinkMap().at("global_pattern_null_sink_mt"));
                    CHECK(logger->level() == spdlog::level::info); // why????
                    CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
                    // no way to really check pattern here
                }

                SUBCASE("global pattern invalid")
                {
                    auto const pat = u8R"(global_pattern = "[%12jh3gz] [%456L]] <<%n>: %%v")"_toml;

                    CHECK_NOTHROW(KDSPDSetup::setup::setupPatterns(pat));

                    auto table = u8R"(
                        name = "global_pattern_invalid"
                        sinks = ["global_pattern_null_sink_mt"]
                    )"_toml.as_table();

                    CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("global_pattern_invalid");
                    CHECK(logger != nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 6);

                    CHECK(logger->sinks().size() == 1);
                    CHECK(logger->sinks()[0] == KDSPDSetup::details::SPDMaps::sinkMap().at("global_pattern_null_sink_mt"));
                    CHECK(logger->level() == spdlog::level::info); // why????
                    CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
                    // no way to really check pattern here
                }

                SUBCASE("global pattern integer")
                {
                    auto const pat = u8R"(global_pattern = 12)"_toml;

                    CHECK_THROWS(KDSPDSetup::setup::setupPatterns(pat)); // this call throws, but the logger can still be created

                    auto table = u8R"(
                        name = "global_pattern_integer"
                        sinks = ["global_pattern_null_sink_mt"]
                    )"_toml.as_table();

                    CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("global_pattern_integer");
                    CHECK(logger != nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 7);

                    CHECK(logger->sinks().size() == 1);
                    CHECK(logger->sinks()[0] == KDSPDSetup::details::SPDMaps::sinkMap().at("global_pattern_null_sink_mt"));
                    CHECK(logger->level() == spdlog::level::info); // why????
                    CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
                }

                SUBCASE("global pattern empty array")
                {
                    auto const pat = u8R"(global_pattern = [])"_toml;

                    CHECK_THROWS(KDSPDSetup::setup::setupPatterns(pat)); // this call throws, but the logger can still be created

                    auto table = u8R"(
                        name = "global_pattern_empty_array"
                        sinks = ["global_pattern_null_sink_mt"]
                    )"_toml.as_table();

                    CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("global_pattern_empty_array");
                    CHECK(logger != nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 8);

                    CHECK(logger->sinks().size() == 1);
                    CHECK(logger->sinks()[0] == KDSPDSetup::details::SPDMaps::sinkMap().at("global_pattern_null_sink_mt"));
                    CHECK(logger->level() == spdlog::level::info); // why????
                    CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
                }

                SUBCASE("global pattern integer array")
                {
                    auto const pat = u8R"(global_pattern = [1, 2, 3])"_toml;

                    CHECK_THROWS(KDSPDSetup::setup::setupPatterns(pat)); // this call throws, but the logger can still be created

                    auto table = u8R"(
                        name = "global_pattern_integer_array"
                        sinks = ["global_pattern_null_sink_mt"]
                    )"_toml.as_table();

                    CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("global_pattern_integer_array");
                    CHECK(logger != nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 9);

                    CHECK(logger->sinks().size() == 1);
                    CHECK(logger->sinks()[0] == KDSPDSetup::details::SPDMaps::sinkMap().at("global_pattern_null_sink_mt"));
                    CHECK(logger->level() == spdlog::level::info); // why????
                    CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
                }

                SUBCASE("global pattern string array")
                {
                    auto const pat = u8R"(global_pattern = ["[%Y-%m-%dT%T%z] [%L] <%n>: %v"])"_toml;

                    CHECK_THROWS(KDSPDSetup::setup::setupPatterns(pat)); // this call throws, but the logger can still be created

                    auto table = u8R"(
                        name = "global_pattern_string_array"
                        sinks = ["global_pattern_null_sink_mt"]
                    )"_toml.as_table();

                    CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("global_pattern_string_array");
                    CHECK(logger != nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array", "global_pattern_string_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 10);

                    CHECK(logger->sinks().size() == 1);
                    CHECK(logger->sinks()[0] == KDSPDSetup::details::SPDMaps::sinkMap().at("global_pattern_null_sink_mt"));
                    CHECK(logger->level() == spdlog::level::info); // why????
                    CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
                }

                SUBCASE("global pattern mixed array")
                {
                    auto const pat = u8R"(global_pattern = ["[%Y-%m-%dT%T%z] [%L] <%n>: %v", 1])"_toml;

                    CHECK_THROWS(KDSPDSetup::setup::setupPatterns(pat)); // this call throws, but the logger can still be created

                    auto table = u8R"(
                        name = "global_pattern_mixed_array"
                        sinks = ["global_pattern_null_sink_mt"]
                    )"_toml.as_table();

                    CHECK_NOTHROW(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("global_pattern_mixed_array");
                    CHECK(logger != nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array", "global_pattern_string_array", "global_pattern_mixed_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 11);

                    CHECK(logger->sinks().size() == 1);
                    CHECK(logger->sinks()[0] == KDSPDSetup::details::SPDMaps::sinkMap().at("global_pattern_null_sink_mt"));
                    CHECK(logger->level() == spdlog::level::info); // why????
                    CHECK_NOTHROW(logger->trace("dlsfjdfjksd"));
                }
            }

            SUBCASE("non-global pattern")
            {
                auto value = u8R"(
                    global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                    [[sink]]
                    name = "non_global_pattern_null_sink_mt"
                    type = "null_sink_mt"

                    [[pattern]]
                    name = "succient"
                    value = "%c-%L: %v"
                )"_toml;

                KDSPDSetup::setup::setupSinks(value);
                KDSPDSetup::setup::setupPatterns(value);

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("non_global_pattern_null_sink_mt"));

                SUBCASE("non-existent pattern")
                {
                    auto table = u8R"(
                        name = "non_existent_pattern"
                        sinks = ["non_global_pattern_null_sink_mt"]
                        pattern = "dshdksfjh"
                    )"_toml.as_table();

                    CHECK_THROWS(KDSPDSetup::setup::setupLogger(table)); // throws due to map::at on name that isnt there
                    auto logger = spdlog::get("non_existent_pattern");
                    CHECK(logger == nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array", "global_pattern_string_array", "global_pattern_mixed_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 11);
                }

                SUBCASE("pattern integer")
                {
                    auto table = u8R"(
                        name = "pattern_integer"
                        sinks = ["non_global_pattern_null_sink_mt"]
                        pattern = 12
                    )"_toml.as_table();

                    CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("pattern_integer");
                    CHECK(logger == nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array", "global_pattern_string_array", "global_pattern_mixed_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 11);
                }

                SUBCASE("pattern empty array")
                {
                    auto table = u8R"(
                        name = "pattern_empty_array"
                        sinks = ["non_global_pattern_null_sink_mt"]
                        pattern = []
                    )"_toml.as_table();

                    CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("pattern_empty_array");
                    CHECK(logger == nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array", "global_pattern_string_array", "global_pattern_mixed_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 11);
                }

                SUBCASE("pattern integer array")
                {
                    auto table = u8R"(
                        name = "pattern_integer_array"
                        sinks = ["non_global_pattern_null_sink_mt"]
                        pattern = [1, 2, 3]
                    )"_toml.as_table();

                    CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("pattern_integer_array");
                    CHECK(logger == nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array", "global_pattern_string_array", "global_pattern_mixed_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 11);
                }

                SUBCASE("pattern string array")
                {
                    auto table = u8R"(
                        name = "pattern_string_array"
                        sinks = ["non_global_pattern_null_sink_mt"]
                        pattern = ["succient"]
                    )"_toml.as_table();

                    CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("pattern_string_array");
                    CHECK(logger == nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array", "global_pattern_string_array", "global_pattern_mixed_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 11);
                }

                SUBCASE("pattern mixed array")
                {
                    auto table = u8R"(
                        name = "pattern_mixed_array"
                        sinks = ["non_global_pattern_null_sink_mt"]
                        pattern = ["succient", 1]
                    )"_toml.as_table();

                    CHECK_THROWS(KDSPDSetup::setup::setupLogger(table));
                    auto logger = spdlog::get("pattern_mixed_array");
                    CHECK(logger == nullptr);

                    int i{ 0 };
                    auto const possible = std::vector<std::string>{ "", "non_existent_sinks", "some_non_existent_sinks", "sinks_empty_array", "no_pattern_and_no_global_pattern", "global_pattern_invalid", "global_pattern_integer", "global_pattern_empty_array", "global_pattern_integer_array", "global_pattern_string_array", "global_pattern_mixed_array" };
                    spdlog::details::registry::instance().apply_all([&i, &possible](auto log) {
                        ++i;
                        CHECK(std::find(possible.cbegin(), possible.cend(), log->name()) != possible.cend());
                    });
                    CHECK(i == 11);
                }
            }
        }

        SUBCASE("type and async")
        {
            SUBCASE("type other than async")
            {
                auto table = u8R"(
                    global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                    [[sink]]
                    name = "null_sink_mt"
                    type = "null_sink_mt"

                    [[logger]]
                    type = "dkjlfhsdk"
                    name = "root"
                    sinks = ["null_sink_mt"]
                )"_toml;

                // test somehow
            }

            SUBCASE("non-async with thread pool")
            {
                auto table = u8R"(
                    global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                    [[sink]]
                    name = "null_sink_mt"
                    type = "null_sink_mt"

                    [[thread_pool]]
                    name = "tp"
                    queue_size = 4096
                    num_threads = 2

                    [[logger]]
                    name = "root"
                    sinks = ["null_sink_mt"]
                    thread_pool = "tp"
                )"_toml;

                // test somehow
            }

            SUBCASE("non-async with overflow policy")
            {
                auto table = u8R"(
                    global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                    [[sink]]
                    name = "null_sink_mt"
                    type = "null_sink_mt"

                    [[logger]]
                    name = "root"
                    sinks = ["null_sink_mt"]
                    overflow_policy = "overrun_oldest"
                )"_toml;

                // test somehow
            }

            SUBCASE("thread pool")
            {
                SUBCASE("no thread pool and no global thread pool")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                    )"_toml;

                    // test somehow
                }

                SUBCASE("invalid thread pool")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = "dfskjfhsdk"
                    )"_toml;

                    // test somehow
                }

                SUBCASE("thread pool integer")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = 12
                    )"_toml;

                    // test somehow
                }

                SUBCASE("thread pool empty array")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = []
                    )"_toml;

                    // test somehow
                }

                SUBCASE("thread pool integer array")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = [1, 2, 3]
                    )"_toml;

                    // test somehow
                }

                SUBCASE("thread pool string array")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = ["tp"]
                    )"_toml;

                    // test somehow
                }

                SUBCASE("thread pool mixed array")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = ["tp", 1]
                    )"_toml;

                    // test somehow
                }
            }

            SUBCASE("overflow policy")
            {
                SUBCASE("invalid overflow policy")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = "tp"
                        overflow_policy = "sdljkkj"
                    )"_toml;

                    // test somehow
                }

                SUBCASE("overflow policy integer")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = "tp"
                        overflow_policy = 12
                    )"_toml;

                    // test somehow
                }

                SUBCASE("overflow policy empty array")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = "tp"
                        overflow_policy = []
                    )"_toml;

                    // test somehow
                }

                SUBCASE("overflow policy integer array")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = "tp"
                        overflow_policy = [1, 2, 3]
                    )"_toml;

                    // test somehow
                }

                SUBCASE("overflow policy string array")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = "tp"
                        overflow_policy = ["overrun_oldest"]
                    )"_toml;

                    // test somehow
                }

                SUBCASE("overflow policy mixed array")
                {
                    auto table = u8R"(
                        global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"

                        [[sink]]
                        name = "null_sink_mt"
                        type = "null_sink_mt"

                        [[thread_pool]]
                        name = "tp"
                        queue_size = 4096
                        num_threads = 2

                        [[logger]]
                        type = "async"
                        name = "root"
                        sinks = ["null_sink_mt"]
                        thread_pool = "tp"
                        overflow_policy = ["overrun_oldest", 1]
                    )"_toml;

                    // test somehow
                }
            }
        }
    }

    TEST_CASE("setupLoggers")
    {
    }

    TEST_CASE("setupSink")
    {
    }

    TEST_CASE("setupSinks")
    {
    }

    TEST_CASE("handleTruncatable")
    {
    }

    TEST_CASE("handleMultifiles")
    {
    }

    TEST_CASE("setupPatterns")
    {
    }

    TEST_CASE("setupThreadPools")
    {
    }

    TEST_CASE("register_async")
    {
    }

    TEST_CASE("registerSyncr")
    {
    }

    TEST_CASE("createRotatingFileSinkStPtr")
    {
    }

    TEST_CASE("createRotatingFileSinkMtPtr")
    {
    }

    TEST_CASE("createFileSinkStPtr")
    {
    }

    TEST_CASE("createFileSinkMtPtr")
    {
    }

    TEST_CASE("createDailyFileSinkStPtr")
    {
    }

    TEST_CASE("createDailyFileSinkMtPtr")
    {
    }

    TEST_CASE("createStdoutSinkStPtr")
    {
    }

    TEST_CASE("createStdoutSinkMtPtr")
    {
    }
#ifdef __linux__
    TEST_CASE("createSyslogSinkStPtr")
    {
    }

    TEST_CASE("createSyslogSinkMtPtr")
    {
    }

    TEST_CASE("createSyslogSinkTuple")
    {
    }

    TEST_CASE("genFromLinuxStr")
    {
    }
#elif _WIN32
    TEST_CASE("createMsvcSinkStPtr")
    {
    }

    TEST_CASE("createMsvcSinkMtPtr")
    {
    }

    TEST_CASE("genFromWinStr")
    {
    }
#endif
    TEST_CASE("createNullSinkPtr")
    {
    }

    TEST_CASE("createStdoutSinkPtr")
    {
    }

    TEST_CASE("inTypelist")
    {
    }

    TEST_CASE("createRotatingFileSinkTuple")
    {
    }

    TEST_CASE("createFileSinkTuple")
    {
    }

    TEST_CASE("createDailyFileSinkTuple")
    {
    }

    TEST_CASE("genFromFileStr")
    {
    }

    TEST_CASE("genFromRotateStr")
    {
    }

    TEST_CASE("genFromDailyStr")
    {
    }

    TEST_CASE("genFromNullOrStdStr")
    {
    }
}
