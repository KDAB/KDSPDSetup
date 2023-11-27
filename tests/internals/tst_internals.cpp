#include "../conf.h"
#include <../src/details.h>
#include <../src/setup.h>
#include <doctest/doctest.h>

TEST_SUITE("Tests for internals")
{
    auto const typeLists = { KDSPDSetup::details::stdStrs, KDSPDSetup::details::fileStrs,
                             KDSPDSetup::details::rotateStrs, KDSPDSetup::details::dailyStrs,
                             KDSPDSetup::details::nullStrs, KDSPDSetup::details::linuxStrs,
                             KDSPDSetup::details::winStrs };

    void checkOnlyIn(std::string && typeStr, std::vector<std::string> const &shouldBeIn)
    {
        for (auto &&type : typeLists) {
            if (type == shouldBeIn) {
                CHECK(KDSPDSetup::details::inTypelist(typeStr, type));
            } else {
                CHECK(!KDSPDSetup::details::inTypelist(typeStr, type));
            }
        }
    }

    TEST_CASE("Check that inTypelist returns true and false as expected.")
    {
        SUBCASE("No partial strings")
        {
            CHECK(!KDSPDSetup::details::inTypelist("file", KDSPDSetup::details::fileStrs));
        }

        SUBCASE("stdStrs")
        {
            auto testStrs = {
                "stdout_sink_st",
                "stdout_sink_mt",
                "stdout_color_sink_st",
                "stdout_color_sink_mt",
                "color_stdout_sink_st",
                "color_stdout_sink_mt",
            };
            for (auto &&str : testStrs) {
                checkOnlyIn(str, KDSPDSetup::details::stdStrs);
            }
        }

        SUBCASE("fileStrs")
        {
            auto testStrs = { "basic_file_sink_st", "basic_file_sink_mt" };
            for (auto &&str : testStrs) {
                checkOnlyIn(str, KDSPDSetup::details::fileStrs);
            }
        }

        SUBCASE("rotateStrs")
        {
            auto testStrs = { "rotating_file_sink_st", "rotating_file_sink_mt" };
            for (auto &&str : testStrs) {
                checkOnlyIn(str, KDSPDSetup::details::rotateStrs);
            }
        }

        SUBCASE("dailyStrs")
        {
            auto testStrs = { "daily_file_sink_st", "daily_file_sink_mt" };
            for (auto &&str : testStrs) {
                checkOnlyIn(str, KDSPDSetup::details::dailyStrs);
            }
        }

        SUBCASE("nullStrs")
        {
            auto testStrs = { "null_sink_st", "null_sink_mt" };
            for (auto &&str : testStrs) {
                checkOnlyIn(str, KDSPDSetup::details::nullStrs);
            }
        }

        SUBCASE("linuxStrs")
        {
            auto testStrs = { "syslog_sink_st", "syslog_sink_mt" };
            for (auto &&str : testStrs) {
                checkOnlyIn(str, KDSPDSetup::details::linuxStrs);
            }
        }

        SUBCASE("winStrs")
        {
            auto testStrs = { "msvc_sink_st", "msvc_sink_mt" };
            for (auto &&str : testStrs) {
                checkOnlyIn(str, KDSPDSetup::details::winStrs);
            }
        }
    }

    TEST_CASE("rotating_file_sink")
    {
        SUBCASE("create_rotating_file_sink_st_ptr")
        {
            toml::table const cTable{ { "name", "rotate_out" },
                                      { "type", "rotating_file_sink_st" },
                                      { "base_filename", "log/rotate_spdlog_setup.log" },
                                      { "max_size", "1M" },
                                      { "max_files", 10 },
                                      { "level", "info" } };

            auto const maxFiles = static_cast<uint16_t>(cTable.at("max_files").as_integer());
            auto const trunct = (cTable.contains("truncate")) ? cTable.at("truncate").as_boolean() : false;

            SUBCASE("tuple")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();

                auto tup = KDSPDSetup::details::createRotatingFileSinkTuple(
                        table, std::move(baseFileName), maxFiles);

                CHECK(std::get<0>(tup) == "log/rotate_spdlog_setup.log");
                CHECK(std::get<1>(tup) == std::size_t{ 1048576 });
                CHECK(std::get<2>(tup) == std::size_t{ 10 });
            }

            SUBCASE("sink_ptr")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();

                auto sinkPtr = KDSPDSetup::details::createRotatingFileSinkStPtr(
                        table, std::move(baseFileName), maxFiles); // still debugging, this needs rewrite

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::rotating_file_sink_st>));
                CHECK(sinkPtr->filename() == "log/rotate_spdlog_setup.log");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("genFromRotateStr")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromRotateStr(std::move(typeStr), std::move(table), std::move(baseFileName), maxFiles);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("handleMultifiles")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleMultipleFileSink(std::move(typeStr), std::move(table), sinkPtr, trunct);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("handleTruncatable")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleTruncatable(std::move(typeStr), std::move(table), sinkPtr);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("rotate_out"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("rotate_out"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("rotate_out");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }
        }

        SUBCASE("create_rotating_file_sink_mt_ptr")
        {
            toml::table cTable{ { "name", "rotate_err" },
                                { "type", "rotating_file_sink_mt" },
                                { "base_filename", "log/rotate_spdlog_setup_err.log" },
                                { "max_size", "1M" },
                                { "max_files", 10 },
                                { "level", "err" } };

            auto const maxFiles = static_cast<std::size_t>(cTable.at("max_files").as_integer());
            auto const trunct = (cTable.contains("truncate")) ? cTable.at("truncate").as_boolean() : false;

            SUBCASE("tuple")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();

                auto tup = KDSPDSetup::details::createRotatingFileSinkTuple(
                        table, std::move(baseFileName), maxFiles);

                CHECK(std::get<0>(tup) == "log/rotate_spdlog_setup_err.log");
                CHECK(std::get<1>(tup) == std::size_t{ 1048576 });
                CHECK(std::get<2>(tup) == std::size_t{ 10 });
            }

            SUBCASE("sink_ptr")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();

                auto sinkPtr = KDSPDSetup::details::createRotatingFileSinkMtPtr(
                        table, std::move(baseFileName), maxFiles); // still debugging, this needs rewrite

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::rotating_file_sink_mt>));
                CHECK(sinkPtr->filename() == "log/rotate_spdlog_setup_err.log");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("genFromRotateStr")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromRotateStr(std::move(typeStr), std::move(table), std::move(baseFileName), maxFiles);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("handleMultifiles")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleMultipleFileSink(std::move(typeStr), std::move(table), sinkPtr, trunct);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("handleTruncatable")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleTruncatable(std::move(typeStr), std::move(table), sinkPtr);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("rotate_err"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("rotate_err"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("rotate_err");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }
        }
    }

    TEST_CASE("basic_file_sink")
    {
        SUBCASE("create_file_sink_st_ptr")
        {
            toml::table cTable{ { "name", "file_out" },
                                { "type", "basic_file_sink_st" },
                                { "filename", "log/spdlog_setup.log" },
                                { "level", "info" },
                                { "create_parent_dir", true } };

            auto const trunct = (cTable.contains("truncate")) ? cTable.at("truncate").as_boolean() : false;

            SUBCASE("tuple")
            {
                auto table = cTable;

                auto tup = KDSPDSetup::details::createFileSinkTuple(table, trunct);

                CHECK(std::get<0>(tup) == "log/spdlog_setup.log");
                CHECK(std::get<1>(tup) == false);
            }

            SUBCASE("sink_ptr")
            {
                auto table = cTable;
                auto baseFileName = table.at("filename").as_string();

                auto sinkPtr = KDSPDSetup::details::createFileSinkStPtr(table, trunct);

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::basic_file_sink_st>));
                CHECK(sinkPtr->filename() == "log/spdlog_setup.log");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("genFrom")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromFileStr(std::move(typeStr), std::move(table), trunct);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("handleTruncatable")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleTruncatable(std::move(typeStr), std::move(table), sinkPtr);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("file_out"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("file_out"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("file_out");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }
        }

        SUBCASE("create_file_sink_mt_ptr")
        {
            toml::table cTable{ { "name", "file_err" },
                                { "type", "basic_file_sink_mt" },
                                { "filename", "log/spdlog_setup_err.log" },
                                { "truncate", true },
                                { "level", "err" } };

            auto const trunct = (cTable.contains("truncate")) ? cTable.at("truncate").as_boolean() : false;

            SUBCASE("tuple")
            {
                auto table = cTable;

                auto tup = KDSPDSetup::details::createFileSinkTuple(table, trunct);

                CHECK(std::get<0>(tup) == "log/spdlog_setup_err.log");
                CHECK(std::get<1>(tup) == true);
            }

            SUBCASE("sink_ptr")
            {
                auto table = cTable;
                auto baseFileName = table.at("filename").as_string();

                auto sinkPtr = KDSPDSetup::details::createFileSinkMtPtr(table, trunct);

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::basic_file_sink_mt>));
                CHECK(sinkPtr->filename() == "log/spdlog_setup_err.log");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("genFrom")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromFileStr(std::move(typeStr), std::move(table), trunct);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("handleTruncatable")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleTruncatable(std::move(typeStr), std::move(table), sinkPtr);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("file_err"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("file_err"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("file_err");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }
        }
    }

    TEST_CASE("daily_file_sink")
    {
        SUBCASE("daily_file_sink_st")
        {
            toml::table cTable{ { "name", "daily_out" },
                                { "type", "daily_file_sink_st" },
                                { "base_filename", "log/daily_spdlog_setup.log" },
                                { "rotation_hour", 17 },
                                { "rotation_minute", 30 },
                                { "level", "info" } };

            auto const maxFiles = (cTable.contains("max_files")) ? static_cast<uint16_t>(cTable.at("max_files").as_integer()) : uint16_t{ 0 };
            auto const trunct = (cTable.contains("truncate")) ? cTable.at("truncate").as_boolean() : false;

            SUBCASE("tuple")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();

                auto tup = KDSPDSetup::details::createDailyFileSinkTuple(std::move(table), trunct, std::move(baseFileName), maxFiles);

                CHECK(std::get<0>(tup) == "log/daily_spdlog_setup.log");
                CHECK(std::get<1>(tup) == int{ 17 });
                CHECK(std::get<2>(tup) == int{ 30 });
                CHECK(std::get<3>(tup) == false);
                CHECK(std::get<4>(tup) == uint16_t{ 0 });
            }

            SUBCASE("sink_ptr")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();

                auto sinkPtr = KDSPDSetup::details::createDailyFileSinkStPtr(std::move(table), trunct, std::move(baseFileName), maxFiles);

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::daily_file_sink_st>));
                // CHECK(sinkPtr->filename() == "log/daily_spdlog_setup.log");
                // not expected behavior - need to figure out how to calculate filename

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("genFrom")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromDailyStr(std::move(typeStr), std::move(table), trunct, std::move(baseFileName), maxFiles);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("handleMultifiles")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleMultipleFileSink(std::move(typeStr), std::move(table), sinkPtr, trunct);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("handleTruncatable")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleTruncatable(std::move(typeStr), std::move(table), sinkPtr);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("daily_out"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("daily_out"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("daily_out");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::info);
            }
        }

        SUBCASE("daily_file_sink_mt")
        {
            toml::table cTable{ { "name", "daily_err" },
                                { "type", "daily_file_sink_mt" },
                                { "base_filename", "log/daily_spdlog_setup_err.log" },
                                { "rotation_hour", 17 },
                                { "rotation_minute", 30 },
                                { "level", "err" } };

            auto const maxFiles = (cTable.contains("max_files")) ? static_cast<uint16_t>(cTable.at("max_files").as_integer()) : uint16_t{ 0 };
            auto const trunct = (cTable.contains("truncate")) ? cTable.at("truncate").as_boolean() : false;

            SUBCASE("tuple")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();

                auto tup = KDSPDSetup::details::createDailyFileSinkTuple(std::move(table), trunct, std::move(baseFileName), maxFiles);

                CHECK(std::get<0>(tup) == "log/daily_spdlog_setup_err.log");
                CHECK(std::get<1>(tup) == int{ 17 });
                CHECK(std::get<2>(tup) == int{ 30 });
                CHECK(std::get<3>(tup) == false);
                CHECK(std::get<4>(tup) == uint16_t{ 0 });
            }

            SUBCASE("sink_ptr")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();

                auto sinkPtr = KDSPDSetup::details::createDailyFileSinkMtPtr(std::move(table), trunct, std::move(baseFileName), maxFiles);

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::daily_file_sink_mt>));
                // CHECK(sinkPtr->filename() == "log/daily_spdlog_setup_err.log");
                // not expected behavior - need to figure out how to calculate filename

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("genFrom")
            {
                auto table = cTable;
                auto baseFileName = table.at("base_filename").as_string();
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromDailyStr(std::move(typeStr), std::move(table), trunct, std::move(baseFileName), maxFiles);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("handleMultifiles")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleMultipleFileSink(std::move(typeStr), std::move(table), sinkPtr, trunct);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("handleTruncatable")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                spdlog::sink_ptr sinkPtr;

                CHECK(sinkPtr == nullptr);

                KDSPDSetup::setup::handleTruncatable(std::move(typeStr), std::move(table), sinkPtr);

                CHECK(sinkPtr != nullptr);

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("daily_err"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("daily_err"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("daily_err");

                auto const level = cTable.at("level").as_string();
                sinkPtr->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sinkPtr->level() == spdlog::level::err);
            }
        }
    }

    TEST_CASE("null_sink_ptr")
    {
        SUBCASE("null_sink_st")
        {
            toml::table cTable{ { "name", "null_sink_st" },
                                { "type", "null_sink_st" } };

            SUBCASE("sink_ptr")
            {
                auto table = cTable;

                auto sinkPtr = KDSPDSetup::details::createNullSinkPtr();

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::null_sink_st>));

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }

            SUBCASE("genFrom")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromNullOrStdStr(std::move(typeStr));

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("null_sink_st"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("null_sink_st"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("null_sink_st");

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }
        }

        SUBCASE("null_sink_mt")
        {
            toml::table cTable{ { "name", "null_sink_mt" },
                                { "type", "null_sink_mt" } };

            SUBCASE("sink_ptr")
            {
                auto table = cTable;

                auto sinkPtr = KDSPDSetup::details::createNullSinkPtr();

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::null_sink_mt>));

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }

            SUBCASE("genFrom")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromNullOrStdStr(std::move(typeStr));

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("null_sink_mt"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("null_sink_mt"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("null_sink_mt");

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }
        }
    }

    TEST_CASE("std_sink_ptr")
    {
        SUBCASE("")
        {
        }
    }
#ifdef __linux__
    TEST_CASE("syslog_sink_ptr")
    {

        SUBCASE("syslog_sink_st")
        {
            toml::table cTable{ { "name", "syslog_st" },
                                { "type", "syslog_sink_st" },
                                { "ident", "example_ident" },
                                { "syslog_option", 1 },
                                { "syslog_facility", 0 } };

            SUBCASE("tuple")
            {
                auto table = cTable;

                auto tup = KDSPDSetup::details::createSyslogSinkTuple(std::move(table));

                CHECK(std::get<0>(tup) == "example_ident");
                CHECK(std::get<1>(tup) == std::size_t{ LOG_PID });
                CHECK(std::get<2>(tup) == std::size_t{ LOG_KERN });
                CHECK(std::get<3>(tup) == true);
            }

            SUBCASE("sink_ptr")
            {
                auto table = cTable;

                auto sinkPtr = KDSPDSetup::details::createSyslogSinkStPtr(std::move(table));

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::syslog_sink_st>));

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }

            SUBCASE("genFrom")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromLinuxStr(std::move(typeStr), std::move(table));

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("syslog_st"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("syslog_st"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("syslog_st");

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }
        }

        SUBCASE("syslog_sink_mt")
        {
            toml::table cTable{ { "name", "syslog_mt" },
                                { "type", "syslog_sink_mt" },
                                { "ident", "another_ident" },
                                { "syslog_option", 0x08 },
                                { "syslog_facility", 16 } };

            SUBCASE("tuple")
            {
                auto table = cTable;

                auto tup = KDSPDSetup::details::createSyslogSinkTuple(std::move(table));

                CHECK(std::get<0>(tup) == "another_ident");
                CHECK(std::get<1>(tup) == std::size_t{ LOG_NDELAY });
                CHECK(std::get<2>(tup) == std::size_t{ LOG_MAIL });
                CHECK(std::get<3>(tup) == true);
            }

            SUBCASE("sink_ptr")
            {
                auto table = cTable;

                auto sinkPtr = KDSPDSetup::details::createSyslogSinkMtPtr(std::move(table));

                CHECK(typeid(sinkPtr) == typeid(std::shared_ptr<spdlog::sinks::syslog_sink_mt>));

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }

            SUBCASE("genFrom")
            {
                auto table = cTable;
                auto typeStr = table.at("type").as_string();

                auto sinkPtr = KDSPDSetup::details::genFromLinuxStr(std::move(typeStr), std::move(table));

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }

            SUBCASE("setupSink")
            {
                auto table = cTable;

                CHECK(!KDSPDSetup::details::SPDMaps::sinkMap().contains("syslog_mt"));

                KDSPDSetup::setup::setupSink(std::move(table));

                CHECK(KDSPDSetup::details::SPDMaps::sinkMap().contains("syslog_mt"));

                auto sinkPtr = KDSPDSetup::details::SPDMaps::sinkMap().at("syslog_mt");

                CHECK(sinkPtr->level() == spdlog::level::trace);
            }
        }
    }
#elif _WIN32
    TEST_CASE("create_msvc_sink_ptr")
    {
        SUBCASE("") { }
    }
#endif
}
