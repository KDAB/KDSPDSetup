#pragma once

#include <map>
#include <vector>

#include <spdlog/async.h>
#include <spdlog/async_logger.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/msvc_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/spdlog.h>

#include <toml.hpp>

namespace KDSPDSetup
{

class KDSPDSetup
{
public:
    static void setup_from(std::string const &);

private:
    // member functions
    ////////////////////////////
    static bool in_typelist(std::string const &typestr, std::string const &str)
    {
        return str.find(typestr) != std::string::npos;
    };

    static void setup_sink(std::map<toml::string, spdlog::sink_ptr> &, toml::table const &);

    static void setup_sinks(toml::value const &, std::map<toml::string, spdlog::sink_ptr> &);

    static void setup_patterns(toml::value const &, std::map<toml::string, toml::string> &);

    static void setup_threadpools(toml::value const &,
                                  std::map<toml::string, std::pair<toml::integer, toml::integer>> &);

    static void register_async(toml::table const &, toml::string const &, std::vector<spdlog::sink_ptr> const &,
                               std::map<toml::string, std::pair<toml::integer, toml::integer>> const &,
                               toml::string const &);

    static void register_sync(toml::table const &, toml::string const &, std::vector<spdlog::sink_ptr> const &,
                              toml::string const &);

    static void setup_logger(toml::table const &, std::map<toml::string, spdlog::sink_ptr> const &,
                             std::map<toml::string, toml::string> const &,
                             std::map<toml::string, std::pair<toml::integer, toml::integer>> const &);

    static void setup_loggers(toml::value const &, std::map<toml::string, spdlog::sink_ptr> const &,
                              std::map<toml::string, toml::string> const &,
                              std::map<toml::string, std::pair<toml::integer, toml::integer>> const &);

    // data members
    ////////////////////////////
    static inline auto const std_strs{std::string{"\
            stdout_sink_st stdout_sink_mt stdout_color_sink_st\
            stdout_color_sink_mt color_stdout_sink_st color_stdout_sink_mt\
        "}};

    static inline auto file_strs{std::string{"basic_file_sink_st basic_file_sink_mt"}};

    static inline auto const rotate_strs{std::string{"rotating_file_sink_st rotating_file_sink_mt"}};

    static inline auto const daily_strs{std::string{"daily_file_sink_st daily_file_sink_mt"}};

    static inline auto const null_strs{std::string{"null_sink_st null_sink_mt"}};

    static inline auto const linux_strs{std::string{"syslog_sink_st syslog_sink_mt"}};

    static inline auto const win_strs{std::string{"msvc_sink_st msvc_sink_mt"}};

    static inline auto const level_map{std::map<std::string, spdlog::level::level_enum>{
        {"critical", spdlog::level::level_enum::critical},
        {"debug", spdlog::level::level_enum::debug},
        {"err", spdlog::level::level_enum::err},
        {"info", spdlog::level::level_enum::info},
        {"n_levels", spdlog::level::level_enum::n_levels},
        {"off", spdlog::level::level_enum::off},
        {"trace", spdlog::level::level_enum::trace},
        {"warn", spdlog::level::level_enum::warn},
    }};

    static inline auto const overflow_map{std::map<toml::string, spdlog::async_overflow_policy>{
        {"overrun_oldest", spdlog::async_overflow_policy::overrun_oldest},
        {"block", spdlog::async_overflow_policy::block},
    }};
};

} // namespace
