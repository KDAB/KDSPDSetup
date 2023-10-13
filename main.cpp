#include <iostream>
#include <vector>
#include <map>

#include <spdlog/spdlog.h>

#include <spdlog/sinks/stdout_sinks.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/syslog_sink.h>
#include <spdlog/sinks/msvc_sink.h>

#include <spdlog/async.h>
// #include <spdlog/async_logger.h>
#include <spdlog/pattern_formatter.h>

#include <toml.hpp>

int main()
{
    auto const data = toml::parse("/home/sus/kdab/kdspdsetup/example.toml");

    auto const stdstrs    = std::string{"stdout_sink_st stdout_sink_mt color_stdout_sink_st color_stdout_sink_mt"};
    auto const filestrs   = std::string{"basic_file_sink_st basic_file_sink_mt"};
    auto const rotatestrs = std::string{"rotating_file_sink_st rotating_file_sink_mt"};
    auto const dailystrs  = std::string{"daily_file_sink_st daily_file_sink_mt"};
    auto const nullstrs   = std::string{"null_sink_st null_sink_mt"};
    auto const linuxstrs  = std::string{"syslog_sink_st syslog_sink_mt"};
    auto const winstrs    = std::string{"msvc_sink_st msvc_sink_mt"};

    static auto levelmap = std::map<std::string, spdlog::level::level_enum>{
        {"critical", spdlog::level::level_enum::critical},
        {"debug",    spdlog::level::level_enum::debug},
        {"err",      spdlog::level::level_enum::err},
        {"info",     spdlog::level::level_enum::info},
        {"n_levels", spdlog::level::level_enum::n_levels},
        {"off",      spdlog::level::level_enum::off},
        {"trace",    spdlog::level::level_enum::trace},
        {"warn",     spdlog::level::level_enum::warn},
    };

    auto const in_typelist = [](std::string const &typestr, std::string const &str) {
        return str.find(typestr) != std::string::npos;
    };

    auto sinkmap       = std::map<toml::string, spdlog::sink_ptr>{};
    auto patternmap    = std::map<toml::string, toml::string>{};
    auto threadpoolmap = std::map<toml::string, std::pair<toml::integer, toml::integer>>{};
    
    auto const sinks = toml::find<std::vector<toml::table>>(data, "sink");
    
    for (auto sinktb : sinks) {
        spdlog::sink_ptr sinkp;

        toml::value const name = sinktb["name"];
        toml::value const type = sinktb["type"];
        auto const typestr = type.as_string();

        if (in_typelist(typestr, filestrs) || in_typelist(typestr, rotatestrs) || in_typelist(typestr, dailystrs)) {
            toml::value const level = sinktb["level"];

            toml::value const trunct = (sinktb.count("truncate") != 0)
                    ? sinktb["truncate"]
                    : false;

            if (in_typelist(typestr, filestrs)) {
                toml::value const filename = sinktb["filename"];
                
                toml::value const create_parent_dir = (sinktb.count("create_parent_dir") != 0)
                        ? sinktb["create_parent_dir"]
                        : false;

                if (typestr == "basic_file_sink_st") {
                    sinkp = std::make_shared<spdlog::sinks::basic_file_sink_st>(
                        filename.as_string(),
                        trunct.as_boolean()
                    );
                } else {
                    sinkp = std::make_shared<spdlog::sinks::basic_file_sink_st>(
                        filename.as_string(),
                        trunct.as_boolean()
                    );
                }
                
                //
                // create parent dir????
                //

            } else if (in_typelist(typestr, rotatestrs) || in_typelist(typestr, dailystrs)) {
                toml::value const base_filename = sinktb["base_filename"];
                toml::value const max_files     = (sinktb.count("max_files") != 0) ? sinktb["max_files"] : 0;
            
                if (in_typelist(typestr, rotatestrs)) {
                    toml::value const max_size  = sinktb["max_size"];
                    
                    auto const max_size_str = static_cast<std::string>(max_size.as_string());
                    char const max_size_str_back = max_size_str.back();
                    size_t max_size_int;
                    
                    if (std::string{"TGMK"}.find(max_size_str_back) != std::string::npos) {
                        auto const prefix_str = max_size_str.substr(0, max_size_str.size() - 1);
                        max_size_int = std::stoi(prefix_str);
                        
                        switch (max_size_str_back) {
                        case 'T':
                            max_size_int *= 1099511627776;
                        case 'G':
                            max_size_int *= 1073741824;
                        case 'M':
                            max_size_int *= 1048576;
                        case 'K':
                            max_size_int *= 1024;
                        default:
                            ;
                        }
                    } else {
                        max_size_int = std::stoi(max_size_str);
                    }
                    
                    if (typestr == "rotating_file_sink_st") {
                        sinkp = std::make_shared<spdlog::sinks::rotating_file_sink_st>(
                            base_filename.as_string(),
                            max_size_int,
                            max_files.as_integer()
                        );
                    } else {
                        sinkp = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                            base_filename.as_string(),
                            max_size_int,
                            max_files.as_integer()
                        );
                    }
                }

                if (in_typelist(typestr, dailystrs)) {
                    toml::value const rotation_hour   = sinktb["rotation_hour"];
                    toml::value const rotation_minute = sinktb["rotation_minute"];
                    toml::value const trunct = (sinktb.count("truncate") != 0)
                        ? sinktb["truncate"]
                        : false;

                    if (typestr == "daily_file_sink_st") {
                        sinkp = std::make_shared<spdlog::sinks::daily_file_sink_st>(
                            base_filename.as_string(),
                            rotation_hour.as_integer(),
                            rotation_minute.as_integer(),
                            trunct.as_boolean(),
                            max_files.as_integer()
                        );
                    } else {
                        sinkp = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                            base_filename.as_string(),
                            rotation_hour.as_integer(),
                            rotation_minute.as_integer(),
                            trunct.as_boolean(),
                            max_files.as_integer()
                        );
                    }
                }
            }

            sinkp->set_level(levelmap[level.as_string()]);

        } else if (in_typelist(typestr, nullstrs) || in_typelist(typestr, stdstrs)) {
            if (typestr == "null_sink_st")
                sinkp = std::make_shared<spdlog::sinks::null_sink_st>();
            else if (typestr == "null_sink_mt")
                sinkp = std::make_shared<spdlog::sinks::null_sink_mt>();
            else if (typestr == "stdout_sink_st")
                sinkp = std::make_shared<spdlog::sinks::stdout_sink_st>();
            else if (typestr == "stdout_sink_mt")
                sinkp = std::make_shared<spdlog::sinks::stdout_sink_st>();
            else if (typestr == "color_stdout_sink_st")
                sinkp = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
            else
                sinkp = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        }
        
        #ifdef __linux__
        else if (in_typelist(typestr, linuxstrs)) {
            toml::value const ident = (sinktb.count("ident") != 0)
                ? sinktb["ident"]
                : "";

            toml::value const syslog_option = (sinktb.count("syslog_option") != 0)
                ? sinktb["syslog_option"]
                : 0;

            toml::value const syslog_facility = (sinktb.count("syslog_facility") != 0)
                ? sinktb["syslog_facility"]
                : LOG_USER;

            toml::value const enable_formatting = true; // not sure if this is correct

            if (typestr == "syslog_sink_st") {
                sinkp = std::make_shared<spdlog::sinks::syslog_sink_st>(
                    ident.as_string(),
                    syslog_option.as_integer(),
                    syslog_facility.as_integer(),
                    enable_formatting.as_boolean()
                );
            } else {
                sinkp = std::make_shared<spdlog::sinks::syslog_sink_mt>(
                    ident.as_string(),
                    syslog_option.as_integer(),
                    syslog_facility.as_integer(),
                    enable_formatting.as_boolean()
                );
            }
        } 
        #elif _WIN32
        else if (in_typelist(typestr, winstrs)) {
            if (typestr == "msvc_sink_st")
                sinkp = std::make_shared<spdlog::sinks::msvc_sink_st>();
            else
                sinkp = std::make_shared<spdlog::sinks::msvc_sink_mt>();
        }
        #endif
    
        sinkmap.emplace(std::make_pair(name.as_string(), sinkp));
    }

    auto const global_pattern = toml::find<toml::string>(data, "global_pattern");

    auto const patterns = toml::find<std::vector<toml::table>>(data, "pattern");
    
    for (auto pattb : patterns) {
        toml::value const name  = pattb["name"];
        toml::value const value = pattb["value"];
        
        patternmap.emplace(std::make_pair(name.as_string(), value.as_string()));
    }

    auto const global_thread_pool = toml::find<toml::string>(data, "global_thread_pool");

    auto const thread_pools = toml::find<std::vector<toml::table>>(data, "thread_pools");
    
    for (auto pooltb : thread_pools) {
        toml::value const name        = pooltb["name"];
        toml::value const queue_size  = pooltb["queue_size"];
        toml::value const num_threads = pooltb["num_threads"];
        
        threadpoolmap.emplace(std::make_pair(
                name.as_string(), 
                std::make_pair(queue_size.as_integer(), num_threads.as_integer())
        ));
    }

    auto const loggers = toml::find<std::vector<toml::table>>(data, "logger");
    for (auto logtb : loggers) {
        toml::value const name = logtb["name"];
        toml::array const sinks = logtb["sinks"].as_array();
        auto sinklist = std::vector<spdlog::sink_ptr>{};
        for (auto sink : sinks) {
            sinklist.emplace_back(sinkmap[name.as_string()]);
        }
        // optional
        toml::value const pattern = logtb.count("pattern") != 0 ? logtb["pattern"] : global_pattern;
        toml::value const level   = logtb.count("level")   != 0 ? logtb["level"]   : /**/;
        toml::value const type    = logtb.count("type")    != 0 ? logtb["type"]    : /**/;
        // async only
        toml::value const thread_pool = logtb["thread_pool"];
        toml::value const overflow_policy = logtb["overflow_policy"];

        auto log = spdlog::logger{name.as_string(), sinklist.cbegin(), sinklist.cend()};
        log.set_level(levelmap[level.as_string()]);
        log.set_pattern(pattern.as_string());
        
        // and what about async?
        // note from example.toml: level is optional for both sinks and loggers

        // reference links
        // https://github.com/ToruNiina/toml11#checking-value-type
        // https://github.com/guangie88/spdlog_setup
        // https://github.com/gabime/spdlog
    }
}
