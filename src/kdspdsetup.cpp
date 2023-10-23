#include "../include/kdspdsetup/kdspdsetup.h"

namespace KDSPDSetup
{

// public member function
////////////////////////////
void KDSPDSetup::setup_from(std::string const &file)
{
    auto const data = toml::parse(file);

    auto sinkmap = std::map<toml::string, spdlog::sink_ptr>{};
    auto patternmap = std::map<toml::string, toml::string>{};
    auto threadpoolmap = std::map<toml::string, std::pair<toml::integer, toml::integer>>{};

    setup_sinks(data, sinkmap);
    setup_patterns(data, patternmap);
    setup_threadpools(data, threadpoolmap);
    setup_loggers(data, sinkmap, patternmap, threadpoolmap);
}

// helpers
////////////////////////////
static auto create_rotating_file_sink_ptr(toml::string const &typestr, toml::table const &sinktb,
                                          toml::string const &base_filename, int const &max_files) -> spdlog::sink_ptr
{
    auto const max_size = static_cast<std::string>(sinktb.at("max_size").as_string());
    auto const max_size_str_back = max_size.back();
    size_t max_size_int;

    if (std::string{"TGMK"}.find(max_size_str_back) != std::string::npos)
    {
        auto const prefix_str = max_size.substr(0, max_size.size() - 1);
        max_size_int = std::stoi(prefix_str);

        size_t constexpr kilo = 1024;
        size_t constexpr mega = kilo * kilo;
        size_t constexpr giga = mega * kilo;
        size_t constexpr tera = giga * kilo;

        switch (max_size_str_back)
        {
        case 'T':
            max_size_int *= tera;
            break;
        case 'G':
            max_size_int *= giga;
            break;
        case 'M':
            max_size_int *= mega;
            break;
        case 'K':
            max_size_int *= kilo;
            break;
        default:
            break;
        }
    }
    else
    {
        max_size_int = std::stoi(max_size);
    }

    if (typestr == "rotating_file_sink_st")
    {
        return std::make_shared<spdlog::sinks::rotating_file_sink_st>(base_filename, max_size_int, max_files);
    }
    return std::make_shared<spdlog::sinks::rotating_file_sink_mt>(base_filename, max_size_int, max_files);
}

static auto create_file_sink_ptr(toml::string const &typestr, toml::table const &sinktb, bool const &trunct)
    -> spdlog::sink_ptr
{
    auto const filename = sinktb.at("filename").as_string();

    if (typestr == "basic_file_sink_st")
    {
        return std::make_shared<spdlog::sinks::basic_file_sink_st>(filename, trunct);
    }

    return std::make_shared<spdlog::sinks::basic_file_sink_st>(filename, trunct);
}

static auto create_daily_file_sink_ptr(toml::string const &typestr, toml::table const &sinktb, bool const &trunct,
                                       toml::string const &base_filename, int const &max_files) -> spdlog::sink_ptr
{
    auto const rotation_hour = sinktb.at("rotation_hour").as_integer();
    auto const rotation_minute = sinktb.at("rotation_minute").as_integer();

    if (typestr == "daily_file_sink_st")
    {
        return std::make_shared<spdlog::sinks::daily_file_sink_st>(base_filename, rotation_hour, rotation_minute,
                                                                   trunct, max_files);
    }
    return std::make_shared<spdlog::sinks::daily_file_sink_mt>(base_filename, rotation_hour, rotation_minute, trunct,
                                                               max_files);
}

static auto create_null_or_std_sink_ptr(toml::string const &typestr) -> spdlog::sink_ptr
{
    if (typestr == "null_sink_st")
    {
        return std::make_shared<spdlog::sinks::null_sink_st>();
    }
    if (typestr == "null_sink_mt")
    {
        return std::make_shared<spdlog::sinks::null_sink_mt>();
    }
    if (typestr == "stdout_sink_st")
    {
        return std::make_shared<spdlog::sinks::stdout_sink_st>();
    }
    if (typestr == "stdout_sink_mt")
    {
        return std::make_shared<spdlog::sinks::stdout_sink_st>();
    }
    if (typestr == "stdout_color_sink_st" or typestr == "color_stdout_sink_st")
    {
        return std::make_shared<spdlog::sinks::stdout_color_sink_st>();
    }
    return std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
}

#ifdef __linux__
static auto create_syslog_sink_ptr(toml::string const &typestr, toml::table const &sinktb) -> spdlog::sink_ptr
{
    auto const ident = (sinktb.contains("ident")) ? sinktb.at("ident").as_string() : "";

    auto const syslog_option = (sinktb.contains("syslog_option")) ? sinktb.at("syslog_option").as_integer() : 0;

    auto const syslog_facility =
        (sinktb.contains("syslog_facility")) ? sinktb.at("syslog_facility").as_integer() : LOG_USER; // macro

    bool const enable_formatting = true;

    if (typestr == "syslog_sink_st")
    {
        return std::make_shared<spdlog::sinks::syslog_sink_st>(ident, syslog_option,
                                                               syslog_facility, // macro
                                                               enable_formatting);
    }

    return std::make_shared<spdlog::sinks::syslog_sink_mt>(ident, syslog_option,
                                                           syslog_facility, // macro
                                                           enable_formatting);
}
#elif _WIN32
auto create_msvc_sink_ptr(toml::string const &typestr) -> spdlog::sink_ptr
{
    if (typestr == "msvc_sink_st")
        return std::make_shared<spdlog::sinks::msvc_sink_st>();

    return std::make_shared<spdlog::sinks::msvc_sink_mt>();
}
#endif

// private member functions
////////////////////////////
void KDSPDSetup::setup_sink(std::map<toml::string, spdlog::sink_ptr> &sinkmap, toml::table const &sinktb)
{
    spdlog::sink_ptr sinkp;

    auto const name = sinktb.at("name").as_string();
    auto const typestr = sinktb.at("type").as_string();

    if (in_typelist(typestr, file_strs) || in_typelist(typestr, rotate_strs) || in_typelist(typestr, daily_strs))
    {
        auto const trunct = (sinktb.contains("truncate")) ? sinktb.at("truncate").as_boolean() : false;

        if (in_typelist(typestr, file_strs))
        {
            sinkp = create_file_sink_ptr(typestr, sinktb, trunct);
        }
        else if (in_typelist(typestr, rotate_strs) or in_typelist(typestr, daily_strs))
        {
            auto const base_filename = sinktb.at("base_filename").as_string();
            auto const max_files = (sinktb.contains("max_files")) ? sinktb.at("max_files").as_integer() : 0;

            if (in_typelist(typestr, rotate_strs))
            {
                sinkp = create_rotating_file_sink_ptr(typestr, sinktb, base_filename, max_files);
            }

            if (in_typelist(typestr, daily_strs))
            {
                sinkp = create_daily_file_sink_ptr(typestr, sinktb, trunct, base_filename, max_files);
            }
        }

        if (sinktb.contains("level"))
        {
            auto const level = sinktb.at("level").as_string();
            sinkp->set_level(level_map.at(level));
        }
    }

    else if (in_typelist(typestr, null_strs) || in_typelist(typestr, std_strs))
    {
        sinkp = create_null_or_std_sink_ptr(typestr);
    }

#ifdef __linux__
    else if (in_typelist(typestr, linux_strs))
    {
        sinkp = create_syslog_sink_ptr(typestr, sinktb);
    }
#elif _WIN32
    else if (in_typelist(typestr, winstrs))
    {
        sinkp = create_msvc_sink_ptr(typestr);
    }
#endif

    sinkmap.emplace(std::make_pair(name, sinkp));
}

void KDSPDSetup::setup_sinks(toml::value const &data, std::map<toml::string, spdlog::sink_ptr> &sinkmap)
{
    auto const &sinks = toml::find<std::vector<toml::table>>(data, "sink");

    for (auto &&sinktb : sinks)
    {
        setup_sink(sinkmap, sinktb);
    }
}

void KDSPDSetup::setup_patterns(toml::value const &data, std::map<toml::string, toml::string> &patternmap)
{
    if (data.contains("global_pattern"))
    {
        auto const global_pattern = toml::find<toml::string>(data, "global_pattern");
        spdlog::set_pattern(global_pattern);
    }

    if (data.contains("pattern"))
    {
        auto const patterns = toml::find<std::vector<toml::table>>(data, "pattern");

        for (auto &&pattb : patterns)
        {
            auto const name = pattb.at("name").as_string();
            auto const value = pattb.at("value").as_string();

            patternmap.emplace(std::make_pair(name, value));
        }
    }
}

void KDSPDSetup::setup_threadpools(toml::value const &data,
                                   std::map<toml::string, std::pair<toml::integer, toml::integer>> &threadpoolmap)
{
    if (data.contains("global_thread_pool"))
    {
        auto const global_thread_pool = toml::find(data, "global_thread_pool");

        spdlog::init_thread_pool(toml::find<toml::integer>(global_thread_pool, "queue_size"),
                                 toml::find<toml::integer>(global_thread_pool, "num_threads"));
    }

    if (data.contains("thread_pool"))
    {
        auto const thread_pools = toml::find<std::vector<toml::table>>(data, "thread_pool");

        for (auto &&pooltb : thread_pools)
        {
            auto const name = pooltb.at("name").as_string();
            auto const queue_size = pooltb.at("queue_size").as_integer();
            auto const num_threads = pooltb.at("num_threads").as_integer();

            threadpoolmap.emplace(std::make_pair(name, std::make_pair(queue_size, num_threads)));
        }
    }
}

void KDSPDSetup::register_async(toml::table const &logtb, toml::string const &name,
                                std::vector<spdlog::sink_ptr> const &sinklist,
                                std::map<toml::string, std::pair<toml::integer, toml::integer>> const &threadpoolmap,
                                toml::string const &pattern)
{
    auto const thread_pool = (logtb.contains("thread_pool")) ? logtb.at("thread_pool").as_string() : "";

    static std::shared_ptr<spdlog::details::thread_pool> tp_ptr;

    if (thread_pool != "")
    {
        auto const tp_pair = threadpoolmap.at(thread_pool);
        auto const queue_size = tp_pair.first;
        auto const num_threads = tp_pair.second;
        tp_ptr = std::make_shared<spdlog::details::thread_pool>(queue_size, num_threads);
    }
    else
    {
        tp_ptr = spdlog::thread_pool();
    }

    auto const overflow_policy =
        (logtb.contains("overflow_policy")) ? logtb.at("overflow_policy").as_string() : "block";

    auto log = std::make_shared<spdlog::async_logger>(name, sinklist.begin(), sinklist.end(), std::move(tp_ptr),
                                                      overflow_map.at(overflow_policy));

    if (pattern != "")
    {
        log->set_pattern(pattern);
    }

    spdlog::register_logger(log);
}

void KDSPDSetup::register_sync(toml::table const &logtb, toml::string const &name,
                               std::vector<spdlog::sink_ptr> const &sinklist, toml::string const &pattern)
{
    auto log = std::make_shared<spdlog::logger>(name, sinklist.cbegin(), sinklist.cend());
    if (pattern != "")
    {
        std::string pats = pattern;
        log->set_pattern(pats);
    }

    if (logtb.contains("level"))
    {
        auto const level = logtb.at("level").as_string();
        log->set_level(level_map.at(level));
    }

    spdlog::register_logger(log);
}

void KDSPDSetup::setup_logger(toml::table const &logtb, std::map<toml::string, spdlog::sink_ptr> const &sinkmap,
                              std::map<toml::string, toml::string> const &patternmap,
                              std::map<toml::string, std::pair<toml::integer, toml::integer>> const &threadpoolmap)
{
    auto const name = logtb.at("name").as_string();
    auto const sinks = logtb.at("sinks").as_array();

    auto sinklist = std::vector<spdlog::sink_ptr>{};
    for (auto &&sink : sinks)
    {
        sinklist.emplace_back(sinkmap.at(sink.as_string()));
    }

    auto const pattern = (logtb.contains("pattern")) ? patternmap.at(logtb.at("pattern").as_string()) : "";

    auto const type = (logtb.contains("type")) ? logtb.at("type").as_string() : "";

    if (type == "async")
    {
        register_async(logtb, name, sinklist, threadpoolmap, pattern);
    }
    else
    {
        register_sync(logtb, name, sinklist, pattern);
    }
}

void KDSPDSetup::setup_loggers(toml::value const &data, std::map<toml::string, spdlog::sink_ptr> const &sinkmap,
                               std::map<toml::string, toml::string> const &patternmap,
                               std::map<toml::string, std::pair<toml::integer, toml::integer>> const &threadpoolmap)
{
    auto const loggers = toml::find<std::vector<toml::table>>(data, "logger");
    for (auto &&logtb : loggers)
    {
        setup_logger(logtb, sinkmap, patternmap, threadpoolmap);
    }
}

} // end namespace