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
#include <spdlog/async_logger.h>
#include <spdlog/pattern_formatter.h>

#include <toml.hpp>

int main()
{
    auto const data = toml::parse("/home/sus/kdab/kdspdsetup/example.toml");

    auto const stdstrs    = std::string{"stdout_sink_st stdout_sink_mt stdout_color_sink_st stdout_color_sink_mt"};
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

    static auto overflowmap = std::map<toml::string, spdlog::async_overflow_policy>{
        {"overrun_oldest", spdlog::async_overflow_policy::overrun_oldest},
        {"block",          spdlog::async_overflow_policy::block},
    };

    static auto const in_typelist = [](std::string const &typestr, std::string const &str) {
        return str.find(typestr) != std::string::npos;
    };

    auto sinkmap       = std::map<toml::string, spdlog::sink_ptr>{};
    auto patternmap    = std::map<toml::string, toml::string>{};
    auto threadpoolmap = std::map<toml::string, std::pair<toml::integer, toml::integer>>{};
    
    auto const sinks = toml::find<std::vector<toml::table>>(data, "sink");
    
    for (auto sinktb : sinks) {
        spdlog::sink_ptr sinkp;

        auto const name = sinktb["name"].as_string();
        auto const typestr = sinktb["type"].as_string();

        if (in_typelist(typestr, filestrs) || in_typelist(typestr, rotatestrs) || in_typelist(typestr, dailystrs)) {
            auto const trunct = (sinktb.count("truncate") != 0)
                    ? sinktb["truncate"].as_boolean()
                    : false;

            if (in_typelist(typestr, filestrs)) {
                auto const filename = sinktb["filename"].as_string();
                
                auto const create_parent_dir = (sinktb.count("create_parent_dir") != 0)
                        ? sinktb["create_parent_dir"].as_boolean()
                        : false;

                if (typestr == "basic_file_sink_st") {
                    sinkp = std::make_shared<spdlog::sinks::basic_file_sink_st>(
                        filename,
                        trunct
                    );
                } else {
                    sinkp = std::make_shared<spdlog::sinks::basic_file_sink_st>(
                        filename,
                        trunct
                    );
                }
                
                //
                // create parent dir????
                //

            } else if (in_typelist(typestr, rotatestrs) || in_typelist(typestr, dailystrs)) {
                auto const base_filename = sinktb["base_filename"].as_string();
                auto const max_files = (sinktb.count("max_files") != 0)
                    ? sinktb["max_files"].as_integer()
                    : 0;
            
                if (in_typelist(typestr, rotatestrs)) {
                    auto const max_size  = sinktb["max_size"];
                    
                    auto const max_size_str = static_cast<std::string>(max_size.as_string());
                    auto const max_size_str_back = max_size_str.back();
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
                            base_filename,
                            max_size_int,
                            max_files
                        );
                    } else {
                        sinkp = std::make_shared<spdlog::sinks::rotating_file_sink_mt>(
                            base_filename,
                            max_size_int,
                            max_files
                        );
                    }
                }

                if (in_typelist(typestr, dailystrs)) {
                    auto const rotation_hour   = sinktb["rotation_hour"].as_integer();
                    auto const rotation_minute = sinktb["rotation_minute"].as_integer();
                    
                    auto const trunct = (sinktb.count("truncate") != 0)
                        ? sinktb["truncate"].as_boolean()
                        : false;

                    if (typestr == "daily_file_sink_st") {
                        sinkp = std::make_shared<spdlog::sinks::daily_file_sink_st>(
                            base_filename,
                            rotation_hour,
                            rotation_minute,
                            trunct,
                            max_files
                        );
                    } else {
                        sinkp = std::make_shared<spdlog::sinks::daily_file_sink_mt>(
                            base_filename,
                            rotation_hour,
                            rotation_minute,
                            trunct,
                            max_files
                        );
                    }
                }
            }

            if (sinktb.count("level") != 0) {
                auto const level = sinktb["level"].as_string();
                sinkp->set_level(levelmap[level]);
            }

        } else if (in_typelist(typestr, nullstrs) || in_typelist(typestr, stdstrs)) {
            if (typestr == "null_sink_st")
                sinkp = std::make_shared<spdlog::sinks::null_sink_st>();
            else if (typestr == "null_sink_mt")
                sinkp = std::make_shared<spdlog::sinks::null_sink_mt>();
            else if (typestr == "stdout_sink_st")
                sinkp = std::make_shared<spdlog::sinks::stdout_sink_st>();
            else if (typestr == "stdout_sink_mt")
                sinkp = std::make_shared<spdlog::sinks::stdout_sink_st>();
            else if (typestr == "stdout_color_sink_st")
                sinkp = std::make_shared<spdlog::sinks::stdout_color_sink_st>();
            else
                sinkp = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
        }
        
        #ifdef __linux__
        else if (in_typelist(typestr, linuxstrs)) {
            auto const ident = (sinktb.count("ident") != 0)
                ? sinktb["ident"].as_string()
                : "";

            auto const syslog_option = (sinktb.count("syslog_option") != 0)
                ? sinktb["syslog_option"].as_integer()
                : 0;

            // auto const syslog_facility = (sinktb.count("syslog_facility") != 0)
            //     ? sinktb["syslog_facility"].as_integer()    // this is actually a macro, needs work
            //     : LOG_USER;

            bool const enable_formatting = true; // not sure if this is correct

            if (typestr == "syslog_sink_st") {
                sinkp = std::make_shared<spdlog::sinks::syslog_sink_st>(
                    ident,
                    syslog_option,
                    LOG_USER,
                    // syslog_facility,    // this is actually a macro, needs work
                    enable_formatting
                );
            } else {
                sinkp = std::make_shared<spdlog::sinks::syslog_sink_mt>(
                    ident,
                    syslog_option,
                    LOG_USER,
                    // syslog_facility,    // this is actually a macro, needs work
                    enable_formatting
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
    
        sinkmap.emplace(std::make_pair(name, sinkp));
    }

    if (data.count("global_pattern") != 0) {
        auto const global_pattern = toml::find<toml::string>(data, "global_pattern");
        spdlog::set_pattern(global_pattern);
    }

    auto const patterns = toml::find<std::vector<toml::table>>(data, "pattern");
    
    for (auto pattb : patterns) {
        auto const name  = pattb["name"].as_string();
        auto const value = pattb["value"].as_string();
        
        patternmap.emplace(std::make_pair(name, value));
    }

    if (data.count("global_thread_pool") != 0) {
        auto const global_thread_pool = toml::find(data, "global_thread_pool");
        spdlog::init_thread_pool(toml::find<toml::integer>(global_thread_pool, "queue_size"),
                                 toml::find<toml::integer>(global_thread_pool, "num_threads"));
    }

    auto const thread_pools = toml::find<std::vector<toml::table>>(data, "thread_pool");
    
    for (auto pooltb : thread_pools) {
        auto const name        = pooltb["name"].as_string();
        auto const queue_size  = pooltb["queue_size"].as_integer();
        auto const num_threads = pooltb["num_threads"].as_integer();
        
        threadpoolmap.emplace(std::make_pair(
            name, 
            std::make_pair(queue_size, num_threads)
        ));
    }

    auto const loggers = toml::find<std::vector<toml::table>>(data, "logger");
    for (auto logtb : loggers) {
        auto const name  = logtb["name"].as_string();
        auto const sinks = logtb["sinks"].as_array();
        
        auto sinklist = std::vector<spdlog::sink_ptr>{};
        for (auto sink : sinks) {
            sinklist.emplace_back(sinkmap[sink.as_string()]);
        }
        
        auto const pattern = (logtb.count("pattern") != 0)
            ? patternmap[logtb["pattern"].as_string()]
            : "";

        auto const type = (logtb.count("type") != 0)
            ? logtb["type"].as_string()
            : "";

        if (type == "async") {
            auto const thread_pool = (logtb.count("thread_pool") != 0)
                ? logtb["thread_pool"].as_string()
                : "";

            std::shared_ptr<spdlog::details::thread_pool> tp;

            if (thread_pool != "") {
                auto const pr = threadpoolmap[thread_pool];
                auto const queue_size  = pr.first;
                auto const num_threads = pr.second;
                tp = std::make_shared<spdlog::details::thread_pool>(queue_size, num_threads);
            } else {
                tp = spdlog::thread_pool();
            }

            auto const overflow_policy = (logtb.count("overflow_policy") != 0)
                ? logtb["overflow_policy"].as_string()
                : "block";
            
            auto log = std::make_shared<spdlog::async_logger>(
                name,
                sinklist.begin(), sinklist.end(),
                tp,
                overflowmap[overflow_policy]
            );
            if (pattern != "")
                log->set_pattern(pattern);

            spdlog::register_logger(log);
        }

        else {
            auto log = std::make_shared<spdlog::logger>(name, sinklist.cbegin(), sinklist.cend());
            if (pattern != "") {
                std::string pats = pattern;
                log->set_pattern(pats);
            }
                

            if (logtb.count("level") != 0) {
                auto const level = logtb["level"].as_string();
                log->set_level(levelmap[level]);
            }

            spdlog::register_logger(log);
        }
    }
}





// reference links
// https://github.com/ToruNiina/toml11#checking-value-type
// https://github.com/guangie88/spdlog_setup
// https://github.com/gabime/spdlog
