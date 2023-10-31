#include "../conf.h"
#include "../src/details.h"
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
            toml::table table{ { "name", "rotate_out" },
                               { "type", "rotating_file_sink_st" },
                               { "base_filename", "log/rotate_spdlog_setup.log" },
                               { "max_size", "1M" },
                               { "max_files", 10 },
                               { "level", "info" } };

            auto moveTupleBaseFileName = table.at("base_filename").as_string();
            auto maxFiles = table.at("max_files").as_integer();

            auto moveSinkBaseFileName = moveTupleBaseFileName;

            SUBCASE("tuple")
            {
                auto tup = KDSPDSetup::details::createRotatingFileSinkTuple(
                        table, std::move(moveTupleBaseFileName), maxFiles);

                CHECK(std::get<0>(tup) == "log/rotate_spdlog_setup.log");
                CHECK(std::get<1>(tup) == std::size_t{ 1048576 });
                CHECK(std::get<2>(tup) == std::size_t{ 10 });
            }

            SUBCASE("sink_ptr")
            {
                auto sink = KDSPDSetup::details::createRotatingFileSinkStPtr(
                        table, std::move(moveSinkBaseFileName), maxFiles); // still debugging, this needs rewrite

                CHECK(typeid(sink) == typeid(std::shared_ptr<spdlog::sinks::rotating_file_sink_st>));
                CHECK(sink->filename() == "log/rotate_spdlog_setup.log");

                auto const level = table.at("level").as_string();
                sink->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sink->level() == spdlog::level::info);
            }
        }

        SUBCASE("create_rotating_file_sink_mt_ptr")
        {
            toml::table table{ { "name", "rotate_err" },
                               { "type", "rotating_file_sink_mt" },
                               { "base_filename", "log/rotate_spdlog_setup_err.log" },
                               { "max_size", "1M" },
                               { "max_files", 10 },
                               { "level", "err" } };

            auto moveTupleBaseFileName = table.at("base_filename").as_string();
            auto maxFiles = table.at("max_files").as_integer();

            auto moveSinkBaseFileName = moveTupleBaseFileName;

            SUBCASE("tuple")
            {
                auto tup = KDSPDSetup::details::createRotatingFileSinkTuple(
                        table, std::move(moveTupleBaseFileName), maxFiles);

                CHECK(std::get<0>(tup) == "log/rotate_spdlog_setup_err.log");
                CHECK(std::get<1>(tup) == std::size_t{ 1048576 });
                CHECK(std::get<2>(tup) == std::size_t{ 10 });
            }

            SUBCASE("sink_ptr")
            {
                auto sink = KDSPDSetup::details::createRotatingFileSinkMtPtr(
                        table, std::move(moveSinkBaseFileName), maxFiles); // still debugging, this needs rewrite

                CHECK(typeid(sink) == typeid(std::shared_ptr<spdlog::sinks::rotating_file_sink_mt>));
                CHECK(sink->filename() == "log/rotate_spdlog_setup_err.log");

                auto const level = table.at("level").as_string();
                sink->set_level(KDSPDSetup::details::levelMap.at(level));

                CHECK(sink->level() == spdlog::level::err);
            }
        }
    }

    TEST_CASE("createFileSinkPtr")
    {
        SUBCASE("create_file_sink_st_ptr")
        {
            toml::table table{ { "name", "file_out" },
                               { "type", "basic_file_sink_st" },
                               { "filename", "log/spdlog_setup.log" },
                               { "level", "info" },
                               { "create_parent_dir", true } };

            auto const trunct = (table.contains("truncate")) ? table.at("truncate").as_boolean() : false;
            auto tup = KDSPDSetup::details::createFileSinkTuple(table, trunct);

            CHECK(std::get<0>(tup) == "log/spdlog_setup.log");
            CHECK(std::get<1>(tup) == false);

            auto sink = KDSPDSetup::details::createFileSinkStPtr(table, trunct);

            CHECK(typeid(sink) == typeid(std::shared_ptr<spdlog::sinks::basic_file_sink_st>));
            CHECK(sink->filename() == "log/spdlog_setup.log");

            auto const level = table.at("level").as_string();
            sink->set_level(KDSPDSetup::details::levelMap.at(level));

            CHECK(sink->level() == spdlog::level::info);
        }

        SUBCASE("create_file_sink_mt_ptr")
        {
            toml::table const table{ { "name", "file_err" },
                                     { "type", "basic_file_sink_mt" },
                                     { "filename", "log/spdlog_setup_err.log" },
                                     { "truncate", true },
                                     { "level", "err" } };

            auto const trunct = (table.contains("truncate")) ? table.at("truncate").as_boolean() : false;
            auto tup = KDSPDSetup::details::createFileSinkTuple(table, trunct);

            CHECK(std::get<0>(tup) == "log/spdlog_setup_err.log");
            CHECK(std::get<1>(tup) == true);

            auto sink = KDSPDSetup::details::createFileSinkMtPtr(table, trunct);

            CHECK(typeid(sink) == typeid(std::shared_ptr<spdlog::sinks::basic_file_sink_mt>));
            CHECK(sink->filename() == "log/spdlog_setup_err.log");

            auto const level = table.at("level").as_string();
            sink->set_level(KDSPDSetup::details::levelMap.at(level));

            CHECK(sink->level() == spdlog::level::err);
        }
    }

    TEST_CASE("createDailyFileSinkPtr")
    {
        SUBCASE("") { }
    }

    TEST_CASE("create_null_or_std_sink_ptr")
    {
        SUBCASE("") { }
    }
#ifdef __linux__
    TEST_CASE("createSyslogSinkPtr")
    {
        SUBCASE("") { }
    }
#elif _WIN32
    TEST_CASE("create_msvc_sink_ptr")
    {
        SUBCASE("") { }
    }
#endif
    TEST_CASE("KDSPDSetup::setupSink")
    {
        // check the sinkMap & such
    }

    // etc...
}
