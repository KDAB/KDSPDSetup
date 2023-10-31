#include "details.h"

namespace KDSPDSetup::details {

auto spd_maps::sinkmap() -> std::map<toml::string, spdlog::sink_ptr> const &
{
    return m_sinkmap;
}
auto spd_maps::patternmap() -> std::map<toml::string, toml::string> const &
{
    return m_patternmap;
}
auto spd_maps::threadpoolmap() -> std::map<toml::string, std::pair<std::size_t, std::size_t>> const &
{
    return m_threadpoolmap;
}
void spd_maps::sinkmap_emplace(std::pair<toml::string, spdlog::sink_ptr> &&_pr)
{
    m_sinkmap.emplace(_pr);
}
void spd_maps::patternmap_emplace(std::pair<toml::string, toml::string> &&_pr)
{
    m_patternmap.emplace(_pr);
}
void spd_maps::threadpoolmap_emplace(std::pair<toml::string, std::pair<std::size_t, std::size_t>> &&_pr)
{
    m_threadpoolmap.emplace(_pr);
}

bool in_typelist(std::string const &typestr, std::vector<std::string> const &strlist)
{
    return std::find(strlist.cbegin(), strlist.cend(), typestr) != std::cend(strlist);
}

auto create_rotating_file_sink_tuple(toml::table const &sinktb, toml::string &&base_filename,
                                     std::size_t const &max_files)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const>
{
    auto const max_size = static_cast<std::string>(sinktb.at("max_size").as_string());
    auto const max_size_str_back = max_size.back();
    std::size_t max_size_int{};

    if (std::string{ "TGMK" }.find(max_size_str_back) != std::string::npos) {
        auto const prefix_str = max_size.substr(0, max_size.size() - 1);
        max_size_int = std::stoi(prefix_str);

        std::size_t constexpr kilo = 1024;
        std::size_t constexpr mega = kilo * kilo;
        std::size_t constexpr giga = mega * kilo;
        std::size_t constexpr tera = giga * kilo;

        switch (max_size_str_back) {
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
    } else {
        max_size_int = std::stoi(max_size);
    }

    return std::make_tuple(std::move(base_filename), max_size_int, max_files);
}

template<typename Mutex>
auto create_rotating_file_sink_ptr(toml::table const &sinktb, toml::string &&base_filename,
                                   std::size_t const &max_files)
        -> std::shared_ptr<spdlog::sinks::rotating_file_sink<Mutex>>
{
    auto tup = create_rotating_file_sink_tuple(sinktb, std::move(base_filename), max_files);
    return std::make_shared<spdlog::sinks::rotating_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup),
                                                                      std::get<2>(tup));
}

auto create_file_sink_tuple(toml::table const &sinktb, bool const &trunct) -> std::tuple<toml::string const, bool const>
{
    auto filename = sinktb.at("filename").as_string();
    return std::make_tuple(std::move(filename), trunct);
}

template<typename Mutex>
auto create_file_sink_ptr(toml::table const &sinktb, bool const &trunct)
        -> std::shared_ptr<spdlog::sinks::basic_file_sink<Mutex>>
{
    auto tup = create_file_sink_tuple(sinktb, trunct);
    return std::make_shared<spdlog::sinks::basic_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup));
}

auto create_daily_file_sink_tuple(toml::table &&sinktb, bool const &trunct, toml::string &&base_filename,
                                  std::size_t const &max_files)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const, bool const, int const>
{
    auto rotation_hour = sinktb.at("rotation_hour").as_integer();
    auto rotation_minute = sinktb.at("rotation_minute").as_integer();

    return std::make_tuple(std::move(base_filename), rotation_hour, rotation_minute, trunct, max_files);
}

template<typename Mutex>
auto create_daily_file_sink_ptr(toml::table &&sinktb, bool const &trunct, toml::string &&base_filename,
                                std::size_t const &max_files) -> std::shared_ptr<spdlog::sinks::daily_file_sink<Mutex>>
{
    auto tup = create_daily_file_sink_tuple(std::move(sinktb), trunct, std::move(base_filename), max_files);
    return std::make_shared<spdlog::sinks::daily_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup),
                                                                   std::get<3>(tup), std::get<4>(tup));
}

auto create_null_sink_ptr() -> std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>>
{
    return std::make_shared<spdlog::sinks::null_sink<spdlog::details::null_mutex>>();
}

template<typename Mutex>
auto create_stdout_sink_ptr() -> std::shared_ptr<spdlog::sinks::stdout_sink<Mutex>>
{
    return std::make_shared<spdlog::sinks::stdout_sink<Mutex>>();
}

//////// needs work
// template<typename Mutex>
// auto create_stdout_color_sink_ptr() -> std::shared_ptr<spdlog::sinks::stdout_color_sink_st<Mutex>>
// {
//     return std::make_shared<spdlog::sinks::stdout_color_sink<Mutex>>();
// }

#ifdef __linux__
auto create_syslog_sink_tuple(toml::table const &sinktb)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const, bool const>
{
    auto ident = (sinktb.contains("ident")) ? sinktb.at("ident").as_string() : "";

    auto syslog_option = (sinktb.contains("syslog_option")) ? sinktb.at("syslog_option").as_integer() : 0;

    auto syslog_facility =
            (sinktb.contains("syslog_facility")) ? sinktb.at("syslog_facility").as_integer() : LOG_USER; // macro

    bool enable_formatting = true;

    return std::make_tuple(std::move(ident), syslog_option, syslog_facility, enable_formatting);
}

template<typename Mutex>
auto create_syslog_sink_ptr(toml::table const &sinktb) -> std::shared_ptr<spdlog::sinks::syslog_sink<Mutex>>
{
    auto tup = create_syslog_sink_tuple(sinktb);
    return std::make_shared<spdlog::sinks::syslog_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup),
                                                               std::get<3>(tup));
}

#elif _WIN32
template<typename Mutex>
auto create_msvc_sink_ptr() -> std::shared_ptr<spdlog::sinks::msvc_sink<Mutex>>
{
    return std::make_shared<spdlog::sinks::msvc_sink<Mutex>>();
}
#endif

auto gen_from_file_str(toml::string &&typestr, toml::table &&sinktb, bool const &trunct) -> spdlog::sink_ptr
{
    if (typestr == "basic_file_sink_st") {
        return create_file_sink_st_ptr(sinktb, trunct);
    }
    if (typestr == "basic_file_sink_mt") {
        return create_file_sink_mt_ptr(sinktb, trunct);
    }

    return nullptr;
}

auto gen_from_rotate_str(toml::string &&typestr, toml::table &&sinktb, toml::string &&base_filename,
                         std::size_t const &max_files) -> spdlog::sink_ptr
{
    if (typestr == "rotating_file_sink_st") {
        return create_rotating_file_sink_st_ptr(std::move(sinktb), std::move(base_filename), max_files);
    }
    if (typestr == "rotating_file_sink_mt") {
        return create_rotating_file_sink_mt_ptr(std::move(sinktb), std::move(base_filename), max_files);
    }

    return nullptr;
}

auto gen_from_daily_str(toml::string &&typestr, toml::table &&sinktb, bool const &trunct,
                        toml::string &&base_filename, std::size_t const &max_files) -> spdlog::sink_ptr
{
    if (typestr == "daily_file_sink_st") {
        return create_daily_file_sink_st_ptr(std::move(sinktb), trunct, std::move(base_filename), max_files);
    }
    if (typestr == "daily_file_sink_mt") {
        return create_daily_file_sink_mt_ptr(std::move(sinktb), trunct, std::move(base_filename), max_files);
    }

    return nullptr;
}

auto gen_from_null_or_std_str(toml::string &&typestr) -> spdlog::sink_ptr
{
    if (typestr == "null_sink_st" || typestr == "null_sink_mt") {
        return create_null_sink_ptr();
    }
    if (typestr == "stdout_sink_st") {
        return create_stdout_sink_st_ptr();
    }
    if (typestr == "stdout_sink_mt") {
        return create_stdout_sink_mt_ptr();
    }
    if (typestr == "stdout_color_sink_st" || typestr == "color_stdout_sink_st") {
        return std::make_shared<spdlog::sinks::stdout_color_sink_st>(); // still needs work
    }
    if (typestr == "stdout_color_sink_mt" || typestr == "color_stdout_sink_mt") {
        return std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); // still needs work
    }

    return nullptr;
}

#ifdef __linux__
auto gen_from_linux_str(toml::string &&typestr, toml::table &&sinktb) -> spdlog::sink_ptr
{
    if (typestr == "syslog_sink_st") {
        return create_syslog_sink_st_ptr(sinktb);
    }
    if (typestr == "syslog_sink_mt") {
        return create_syslog_sink_mt_ptr(sinktb);
    }

    return nullptr;
}
#elif _WIN32
static auto gen_from_win_str(toml::string &&typestr) -> spdlog::sink_ptr
{
    if (typestr == "msvc_sink_st") {
        return create_msvc_sink_st_ptr();
    }
    if (typestr == "msvc_sink_mt") {
        return create_msvc_sink_mt_ptr();
    }

    return nullptr;
}
#endif

} // namespace KDSPDSetup::details
