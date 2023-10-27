#include "setup.h"

namespace KDSPDSetup::setup
{

void handle_multifiles(toml::string &&typestr, toml::table const &sinktb, spdlog::sink_ptr &sinkp, bool const &trunct)
{
    auto base_filename = sinktb.at("base_filename").as_string();
    auto max_files = (sinktb.contains("max_files")) ? sinktb.at("max_files").as_integer() : 0;

    if (details::in_typelist(typestr, details::rotate_strs))
    {
        sinkp = details::gen_from_rotate_str(std::move(typestr), sinktb, std::move(base_filename), max_files);
    }

    else if (details::in_typelist(typestr, details::daily_strs))
    {
        sinkp = details::gen_from_daily_str(std::move(typestr), sinktb, trunct, std::move(base_filename), max_files);
    }
}

void handle_truncatable(toml::string &&typestr, toml::table &&sinktb, spdlog::sink_ptr &sinkp)
{
    auto const trunct = (sinktb.contains("truncate")) ? sinktb.at("truncate").as_boolean() : false;

    if (details::in_typelist(typestr, details::file_strs))
    {
        sinkp = details::gen_from_file_str(std::move(typestr), sinktb, trunct);
    }
    else if (details::in_typelist(typestr, details::rotate_strs) || details::in_typelist(typestr, details::daily_strs))
    {
        handle_multifiles(std::move(typestr), sinktb, sinkp, trunct);
    }

    if (sinktb.contains("level"))
    {
        auto const level = sinktb.at("level").as_string();
        sinkp->set_level(details::level_map.at(level));
    }
}

void setup_sink(toml::table &&sinktb)
{
    spdlog::sink_ptr sinkp;

    auto const name = sinktb.at("name").as_string();
    auto typestr = sinktb.at("type").as_string();

    if (details::in_typelist(typestr, details::file_strs) || details::in_typelist(typestr, details::rotate_strs) ||
        details::in_typelist(typestr, details::daily_strs))
    {
        handle_truncatable(std::move(typestr), std::move(sinktb), sinkp);
    }

    else if (details::in_typelist(typestr, details::null_strs) || details::in_typelist(typestr, details::std_strs))
    {
        sinkp = details::gen_from_null_or_std_str(std::move(typestr));
    }

    else if (details::in_typelist(typestr, details::linux_strs))
    {
#ifdef __linux__
        sinkp = details::gen_from_linux_str(std::move(typestr), sinktb);
#else
        return;
#endif
    }
    else if (details::in_typelist(typestr, details::win_strs))
    {
#ifdef _WIN32
        sinkp = details::create_msvc_sink_ptr();
#else
        return;
#endif
    }

    details::spd_maps::sinkmap_emplace(std::make_pair(name, sinkp));
}

void setup_sinks(toml::value const &data)
{
    auto sinks = toml::find<std::vector<toml::table>>(data, "sink");

    for (auto &&sinktb : sinks)
    {
        setup_sink(std::move(sinktb));
    }
}

void setup_patterns(toml::value const &data)
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

            details::spd_maps::patternmap_emplace(std::make_pair(name, value));
        }
    }
}

void setup_threadpools(toml::value const &data)
{
    if (data.contains("global_thread_pool"))
    {
        auto const global_thread_pool = toml::find(data, "global_thread_pool");

        auto const qsize = toml::find<std::size_t>(global_thread_pool, "queue_size");
        auto const nthreads = toml::find<std::size_t>(global_thread_pool, "num_threads");
        spdlog::init_thread_pool(qsize, nthreads);
    }

    if (data.contains("thread_pool"))
    {
        auto const thread_pools = toml::find<std::vector<toml::table>>(data, "thread_pool");

        for (auto &&pooltb : thread_pools)
        {
            auto const name = pooltb.at("name").as_string();
            auto const queue_size = pooltb.at("queue_size").as_integer();
            auto const num_threads = pooltb.at("num_threads").as_integer();

            details::spd_maps::threadpoolmap_emplace(std::make_pair(name, std::make_pair(queue_size, num_threads)));
        }
    }
}

void register_async(toml::table const &logtb, toml::string const &name, std::vector<spdlog::sink_ptr> const &sinklist,
                    toml::string const &pattern)
{
    auto const thread_pool = (logtb.contains("thread_pool")) ? logtb.at("thread_pool").as_string() : "";

    static std::shared_ptr<spdlog::details::thread_pool> tp_ptr;

    if (thread_pool != "")
    {
        auto const tp_pair = details::spd_maps::threadpoolmap().at(thread_pool);
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
                                                      details::overflow_map.at(overflow_policy));

    if (pattern != "")
    {
        log->set_pattern(pattern);
    }

    spdlog::register_logger(log);
}

void register_syncr(toml::table const &logtb, toml::string const &name, std::vector<spdlog::sink_ptr> const &sinklist,
                    toml::string const &pattern)
{
    auto log = std::make_shared<spdlog::logger>(name, sinklist.cbegin(), sinklist.cend());
    if (pattern != "")
    {
        std::string const &pats = pattern;
        log->set_pattern(pats);
    }

    if (logtb.contains("level"))
    {
        auto const level = logtb.at("level").as_string();
        log->set_level(details::level_map.at(level));
    }

    spdlog::register_logger(log);
}

void setup_logger(toml::table const &logtb)
{
    auto const name = logtb.at("name").as_string();
    if (spdlog::get(name) != nullptr)
    {
        return;
    }

    auto const sinks = logtb.at("sinks").as_array();

    auto sinklist = std::vector<spdlog::sink_ptr>{};
    for (auto &&sink : sinks)
    {
        if (details::spd_maps::sinkmap().contains(sink.as_string()))
        {
            sinklist.emplace_back(details::spd_maps::sinkmap().at(sink.as_string()));
        }
    }

    auto const pattern =
        (logtb.contains("pattern")) ? details::spd_maps::patternmap().at(logtb.at("pattern").as_string()) : "";

    auto const type = (logtb.contains("type")) ? logtb.at("type").as_string() : "";

    if (type == "async")
    {
        register_async(logtb, name, sinklist, pattern);
    }
    else
    {
        register_syncr(logtb, name, sinklist, pattern);
    }
}

void setup_loggers(toml::value const &data)
{
    auto const loggers = toml::find<std::vector<toml::table>>(data, "logger");
    for (auto &&logtb : loggers)
    {
        setup_logger(logtb);
    }
}

} // namespace KDSPDSetup
