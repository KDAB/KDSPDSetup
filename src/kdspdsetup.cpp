#include "../include/kdspdsetup/kdspdsetup.h"

namespace KDSPDSetup {

void KDSPDSetup::setup_from (std::string const &file) {
    auto const data = toml::parse(file);
    
    auto sinkmap       = std::map<toml::string, spdlog::sink_ptr>{};
    auto patternmap    = std::map<toml::string, toml::string>{};
    auto threadpoolmap = std::map<toml::string, std::pair<toml::integer, toml::integer>>{};

    setup_sinks      (data, sinkmap);
    setup_patterns   (data, patternmap);
    setup_threadpools(data, threadpoolmap);
    setup_loggers    (data, sinkmap, patternmap, threadpoolmap);
}

void
KDSPDSetup::setup_sinks (
    toml::value const &data,
    std::map<toml::string, spdlog::sink_ptr> &sinkmap)
{
    auto const &sinks = toml::find<std::vector<toml::table>>(data, "sink");

    for (auto sinktb : sinks) {
        spdlog::sink_ptr sinkp;

        auto const name    = sinktb["name"].as_string();
        auto const typestr = sinktb["type"].as_string();

        if (in_typelist(typestr, filestrs)   ||
            in_typelist(typestr, rotatestrs) || 
            in_typelist(typestr, dailystrs))
        {
            auto const trunct = (sinktb.count("truncate") != 0)
                    ? sinktb["truncate"].as_boolean()
                    : false;

            if (in_typelist(typestr, filestrs)) {
                auto const filename = sinktb["filename"].as_string();

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

            } else if (in_typelist(typestr, rotatestrs) or in_typelist(typestr, dailystrs)) {
                auto const base_filename = sinktb["base_filename"].as_string();
                auto const max_files = (sinktb.count("max_files") != 0)
                    ? sinktb["max_files"].as_integer()
                    : 0;

                if (in_typelist(typestr, rotatestrs)) {
                    auto const max_size = static_cast<std::string>(sinktb["max_size"].as_string());

                    auto const max_size_str_back = max_size.back();
                    size_t max_size_int;

                    if (std::string{"TGMK"}.find(max_size_str_back) != std::string::npos) {
                        auto const prefix_str = max_size.substr(0, max_size.size() - 1);
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
                        max_size_int = std::stoi(max_size);
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
                sinkp->set_level(levelmap.at(level));
            }

        }

        else if (in_typelist(typestr, nullstrs) or in_typelist(typestr, stdstrs)) {
            if (typestr == "null_sink_st")
                sinkp = std::make_shared<spdlog::sinks::null_sink_st>();
            else if (typestr == "null_sink_mt")
                sinkp = std::make_shared<spdlog::sinks::null_sink_mt>();
            else if (typestr == "stdout_sink_st")
                sinkp = std::make_shared<spdlog::sinks::stdout_sink_st>();
            else if (typestr == "stdout_sink_mt")
                sinkp = std::make_shared<spdlog::sinks::stdout_sink_st>();
            else if (typestr == "stdout_color_sink_st" or typestr == "color_stdout_sink_st")
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

            auto const syslog_facility = (sinktb.count("syslog_facility") != 0)
                ? sinktb["syslog_facility"].as_integer()
                : LOG_USER;             // macro

            bool const enable_formatting = true;

            if (typestr == "syslog_sink_st") {
                sinkp = std::make_shared<spdlog::sinks::syslog_sink_st>(
                    ident,
                    syslog_option,
                    syslog_facility,    // macro
                    enable_formatting
                );
            } else {
                sinkp = std::make_shared<spdlog::sinks::syslog_sink_mt>(
                    ident,
                    syslog_option,
                    syslog_facility,    // macro
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
}

void
KDSPDSetup::setup_patterns (
    toml::value const &data,
    std::map<toml::string, toml::string> &patternmap)
{
    if (data.count("global_pattern") != 0) {
        auto const global_pattern = toml::find<toml::string>(data, "global_pattern");
        spdlog::set_pattern(global_pattern);
    }

    if (data.count("patterns") != 0) {
        auto const patterns = toml::find<std::vector<toml::table>>(data, "pattern");

        for (auto pattb : patterns) {
            auto const name  = pattb["name"].as_string();
            auto const value = pattb["value"].as_string();
            
            patternmap.emplace(std::make_pair(name, value));
        }
    }
}

void
KDSPDSetup::setup_threadpools (
    toml::value const &data,
    std::map<toml::string, std::pair<toml::integer, toml::integer>> &threadpoolmap)
{
    if (data.count("global_thread_pool") != 0) {
        auto const global_thread_pool = toml::find(data, "global_thread_pool");
        
        spdlog::init_thread_pool(
            toml::find<toml::integer>(global_thread_pool, "queue_size"),
            toml::find<toml::integer>(global_thread_pool, "num_threads")
        );
    }

    if (data.count("thread_pool") != 0) {
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
    }
}

void
KDSPDSetup::setup_loggers (
    toml::value const &data,
    std::map<toml::string, spdlog::sink_ptr> &sinkmap,
    std::map<toml::string, toml::string> &patternmap,
    std::map<toml::string, std::pair<toml::integer, toml::integer>> &threadpoolmap)
{
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

            static std::shared_ptr<spdlog::details::thread_pool> tp;

            if (thread_pool != "") {
                auto const pr = threadpoolmap[thread_pool];
                auto const queue_size  = pr.first;
                auto const num_threads = pr.second;
                tp = std::make_shared<spdlog::details::thread_pool>(
                    queue_size,
                    num_threads
                );
            } else {
                tp = spdlog::thread_pool();
            }

            auto const overflow_policy = (logtb.count("overflow_policy") != 0)
                ? logtb["overflow_policy"].as_string()
                : "block";
            
            auto log = std::make_shared<spdlog::async_logger>(
                name,
                sinklist.begin(), sinklist.end(),
                std::move(tp),
                overflowmap.at(overflow_policy)
            );

            if (pattern != "")
                log->set_pattern(pattern);

            spdlog::register_logger(log);

        } else {
            auto log = std::make_shared<spdlog::logger>(name, sinklist.cbegin(), sinklist.cend());
            if (pattern != "") {
                std::string pats = pattern;
                log->set_pattern(pats);
            }

            if (logtb.count("level") != 0) {
                auto const level = logtb["level"].as_string();
                log->set_level(levelmap.at(level));
            }

            spdlog::register_logger(log);
        }
    }
}

} // end namespace