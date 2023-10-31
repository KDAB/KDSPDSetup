#include "../conf.h"
#include "../src/details.h"
#include <doctest/doctest.h>

TEST_SUITE("Tests for internals")
{
    auto const typelists = { KDSPDSetup::details::std_strs, KDSPDSetup::details::file_strs,
                             KDSPDSetup::details::rotate_strs, KDSPDSetup::details::daily_strs,
                             KDSPDSetup::details::null_strs, KDSPDSetup::details::linux_strs,
                             KDSPDSetup::details::win_strs };

    void check_only_in(std::string && typestr, std::vector<std::string> const &should_be_in)
    {
        for (auto &&typ : typelists) {
            if (typ == should_be_in) {
                CHECK(KDSPDSetup::details::in_typelist(typestr, typ));
            } else {
                CHECK(!KDSPDSetup::details::in_typelist(typestr, typ));
            }
        }
    }

    TEST_CASE("Check that in_typelist returns true and false as expected.")
    {
        SUBCASE("No partial strings")
        {
            CHECK(!KDSPDSetup::details::in_typelist("file", KDSPDSetup::details::file_strs));
        }

        SUBCASE("std_strs")
        {
            auto test_strs = {
                "stdout_sink_st",
                "stdout_sink_mt",
                "stdout_color_sink_st",
                "stdout_color_sink_mt",
                "color_stdout_sink_st",
                "color_stdout_sink_mt",
            };
            for (auto &&str : test_strs) {
                check_only_in(str, KDSPDSetup::details::std_strs);
            }
        }

        SUBCASE("file_strs")
        {
            auto test_strs = { "basic_file_sink_st", "basic_file_sink_mt" };
            for (auto &&str : test_strs) {
                check_only_in(str, KDSPDSetup::details::file_strs);
            }
        }

        SUBCASE("rotate_strs")
        {
            auto test_strs = { "rotating_file_sink_st", "rotating_file_sink_mt" };
            for (auto &&str : test_strs) {
                check_only_in(str, KDSPDSetup::details::rotate_strs);
            }
        }

        SUBCASE("daily_strs")
        {
            auto test_strs = { "daily_file_sink_st", "daily_file_sink_mt" };
            for (auto &&str : test_strs) {
                check_only_in(str, KDSPDSetup::details::daily_strs);
            }
        }

        SUBCASE("null_strs")
        {
            auto test_strs = { "null_sink_st", "null_sink_mt" };
            for (auto &&str : test_strs) {
                check_only_in(str, KDSPDSetup::details::null_strs);
            }
        }

        SUBCASE("linux_strs")
        {
            auto test_strs = { "syslog_sink_st", "syslog_sink_mt" };
            for (auto &&str : test_strs) {
                check_only_in(str, KDSPDSetup::details::linux_strs);
            }
        }

        SUBCASE("win_strs")
        {
            auto test_strs = { "msvc_sink_st", "msvc_sink_mt" };
            for (auto &&str : test_strs) {
                check_only_in(str, KDSPDSetup::details::win_strs);
            }
        }
    }

    TEST_CASE("rotating_file_sink")
    {
        SUBCASE("create_rotating_file_sink_st_ptr")
        {
            toml::table tbl{ { "name", "rotate_out" },
                             { "type", "rotating_file_sink_st" },
                             { "base_filename", "log/rotate_spdlog_setup.log" },
                             { "max_size", "1M" },
                             { "max_files", 10 },
                             { "level", "info" } };

            auto move_tuple_base_filename = tbl.at("base_filename").as_string();
            auto max_files = tbl.at("max_files").as_integer();

            auto move_sink_base_filename = move_tuple_base_filename;

            SUBCASE("tuple")
            {
                auto tup = KDSPDSetup::details::create_rotating_file_sink_tuple(
                        tbl, std::move(move_tuple_base_filename), max_files);

                CHECK(std::get<0>(tup) == "log/rotate_spdlog_setup.log");
                CHECK(std::get<1>(tup) == std::size_t{ 1048576 });
                CHECK(std::get<2>(tup) == std::size_t{ 10 });
            }

            SUBCASE("sink_ptr")
            {
                auto snk = KDSPDSetup::details::create_rotating_file_sink_st_ptr(
                        tbl, std::move(move_sink_base_filename), max_files); // still debugging, this needs rewrite

                CHECK(typeid(snk) == typeid(std::shared_ptr<spdlog::sinks::rotating_file_sink_st>));
                CHECK(snk->filename() == "log/rotate_spdlog_setup.log");

                auto const level = tbl.at("level").as_string();
                snk->set_level(KDSPDSetup::details::level_map.at(level));

                CHECK(snk->level() == spdlog::level::info);
            }
        }

        SUBCASE("create_rotating_file_sink_mt_ptr")
        {
            toml::table tbl{ { "name", "rotate_err" },
                             { "type", "rotating_file_sink_mt" },
                             { "base_filename", "log/rotate_spdlog_setup_err.log" },
                             { "max_size", "1M" },
                             { "max_files", 10 },
                             { "level", "err" } };

            auto move_tuple_base_filename = tbl.at("base_filename").as_string();
            auto max_files = tbl.at("max_files").as_integer();

            auto move_sink_base_filename = move_tuple_base_filename;

            SUBCASE("tuple")
            {
                auto tup = KDSPDSetup::details::create_rotating_file_sink_tuple(
                        tbl, std::move(move_tuple_base_filename), max_files);

                CHECK(std::get<0>(tup) == "log/rotate_spdlog_setup_err.log");
                CHECK(std::get<1>(tup) == std::size_t{ 1048576 });
                CHECK(std::get<2>(tup) == std::size_t{ 10 });
            }

            SUBCASE("sink_ptr")
            {
                auto snk = KDSPDSetup::details::create_rotating_file_sink_mt_ptr(
                        tbl, std::move(move_sink_base_filename), max_files); // still debugging, this needs rewrite

                CHECK(typeid(snk) == typeid(std::shared_ptr<spdlog::sinks::rotating_file_sink_mt>));
                CHECK(snk->filename() == "log/rotate_spdlog_setup_err.log");

                auto const level = tbl.at("level").as_string();
                snk->set_level(KDSPDSetup::details::level_map.at(level));

                CHECK(snk->level() == spdlog::level::err);
            }
        }
    }

    TEST_CASE("create_file_sink_ptr")
    {
        SUBCASE("create_file_sink_st_ptr")
        {
            toml::table tbl{ { "name", "file_out" },
                             { "type", "basic_file_sink_st" },
                             { "filename", "log/spdlog_setup.log" },
                             { "level", "info" },
                             { "create_parent_dir", true } };

            auto const trunct = (tbl.contains("truncate")) ? tbl.at("truncate").as_boolean() : false;
            auto tup = KDSPDSetup::details::create_file_sink_tuple(tbl, trunct);

            CHECK(std::get<0>(tup) == "log/spdlog_setup.log");
            CHECK(std::get<1>(tup) == false);

            auto snk = KDSPDSetup::details::create_file_sink_st_ptr(tbl, trunct);

            CHECK(typeid(snk) == typeid(std::shared_ptr<spdlog::sinks::basic_file_sink_st>));
            CHECK(snk->filename() == "log/spdlog_setup.log");

            auto const level = tbl.at("level").as_string();
            snk->set_level(KDSPDSetup::details::level_map.at(level));

            CHECK(snk->level() == spdlog::level::info);
        }

        SUBCASE("create_file_sink_mt_ptr")
        {
            toml::table const tbl{ { "name", "file_err" },
                                   { "type", "basic_file_sink_mt" },
                                   { "filename", "log/spdlog_setup_err.log" },
                                   { "truncate", true },
                                   { "level", "err" } };

            auto const trunct = (tbl.contains("truncate")) ? tbl.at("truncate").as_boolean() : false;
            auto tup = KDSPDSetup::details::create_file_sink_tuple(tbl, trunct);

            CHECK(std::get<0>(tup) == "log/spdlog_setup_err.log");
            CHECK(std::get<1>(tup) == true);

            auto snk = KDSPDSetup::details::create_file_sink_mt_ptr(tbl, trunct);

            CHECK(typeid(snk) == typeid(std::shared_ptr<spdlog::sinks::basic_file_sink_mt>));
            CHECK(snk->filename() == "log/spdlog_setup_err.log");

            auto const level = tbl.at("level").as_string();
            snk->set_level(KDSPDSetup::details::level_map.at(level));

            CHECK(snk->level() == spdlog::level::err);
        }
    }

    TEST_CASE("create_daily_file_sink_ptr")
    {
        SUBCASE("") { }
    }

    TEST_CASE("create_null_or_std_sink_ptr")
    {
        SUBCASE("") { }
    }
#ifdef __linux__
    TEST_CASE("create_syslog_sink_ptr")
    {
        SUBCASE("") { }
    }
#elif _WIN32
    TEST_CASE("create_msvc_sink_ptr")
    {
        SUBCASE("") { }
    }
#endif
    TEST_CASE("KDSPDSetup::setup_sink")
    {
        // check the sinkmap & such
    }

    // etc...
}
