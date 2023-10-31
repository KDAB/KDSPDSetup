#pragma once

#include "includes.h"

namespace KDSPDSetup::details {

static auto const std_strs{ std::vector<std::string>{ "stdout_sink_st", "stdout_sink_mt", "stdout_color_sink_st",
                                                      "stdout_color_sink_mt", "color_stdout_sink_st",
                                                      "color_stdout_sink_mt" } };
static auto const file_strs{ std::vector<std::string>{ "basic_file_sink_st", "basic_file_sink_mt" } };
static auto const rotate_strs{ std::vector<std::string>{ "rotating_file_sink_st", "rotating_file_sink_mt" } };
static auto const daily_strs{ std::vector<std::string>{ "daily_file_sink_st", "daily_file_sink_mt" } };
static auto const null_strs{ std::vector<std::string>{ "null_sink_st", "null_sink_mt" } };
static auto const linux_strs{ std::vector<std::string>{ "syslog_sink_st", "syslog_sink_mt" } };
static auto const win_strs{ std::vector<std::string>{ "msvc_sink_st", "msvc_sink_mt" } };

static auto const level_map{ std::unordered_map<std::string, spdlog::level::level_enum>{
        { "critical", spdlog::level::level_enum::critical },
        { "debug", spdlog::level::level_enum::debug },
        { "err", spdlog::level::level_enum::err },
        { "info", spdlog::level::level_enum::info },
        { "n_levels", spdlog::level::level_enum::n_levels },
        { "off", spdlog::level::level_enum::off },
        { "trace", spdlog::level::level_enum::trace },
        { "warn", spdlog::level::level_enum::warn },
} };

static auto const overflow_map{ std::map<toml::string, spdlog::async_overflow_policy>{
        { "overrun_oldest", spdlog::async_overflow_policy::overrun_oldest },
        { "block", spdlog::async_overflow_policy::block },
} };

class spd_maps
{
public:
    static auto sinkmap() -> std::map<toml::string, spdlog::sink_ptr> const &;
    static auto patternmap() -> std::map<toml::string, toml::string> const &;
    static auto threadpoolmap() -> std::map<toml::string, std::pair<std::size_t, std::size_t>> const &;
    static void sinkmap_emplace(std::pair<toml::string, spdlog::sink_ptr> &&_pr);
    static void patternmap_emplace(std::pair<toml::string, toml::string> &&_pr);
    static void threadpoolmap_emplace(std::pair<toml::string, std::pair<std::size_t, std::size_t>> &&_pr);

private:
    static inline std::map<toml::string, spdlog::sink_ptr> m_sinkmap{};
    static inline std::map<toml::string, toml::string> m_patternmap{};
    static inline std::map<toml::string, std::pair<std::size_t, std::size_t>> m_threadpoolmap{};
};

#define create_rotating_file_sink_st_ptr create_rotating_file_sink_ptr<spdlog::details::null_mutex>
#define create_rotating_file_sink_mt_ptr create_rotating_file_sink_ptr<std::mutex>

#define create_file_sink_st_ptr create_file_sink_ptr<spdlog::details::null_mutex>
#define create_file_sink_mt_ptr create_file_sink_ptr<std::mutex>

#define create_daily_file_sink_st_ptr create_daily_file_sink_ptr<spdlog::details::null_mutex>
#define create_daily_file_sink_mt_ptr create_daily_file_sink_ptr<std::mutex>

#define create_stdout_sink_st_ptr create_stdout_sink_ptr<spdlog::details::console_nullmutex>
#define create_stdout_sink_mt_ptr create_stdout_sink_ptr<spdlog::details::console_mutex>

#ifdef __linux__
#define create_syslog_sink_st_ptr create_syslog_sink_ptr<spdlog::details::null_mutex>
#define create_syslog_sink_mt_ptr create_syslog_sink_ptr<std::mutex>
#elif _WIN32
#define create_msvc_sink_st_ptr create_msvc_sink_ptr<spdlog::null_mutex>
#define create_msvc_sink_mt_ptr create_msvc_sink_ptr<std::mutex>
#endif

bool in_typelist(std::string const &typestr, std::vector<std::string> const &strlist);

auto create_rotating_file_sink_tuple(toml::table const &sinktb, toml::string &&base_filename,
                                     std::size_t const &max_files)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const>;

template<typename Mutex>
auto create_rotating_file_sink_ptr(toml::table const &sinktb, toml::string &&base_filename,
                                   std::size_t const &max_files)
        -> std::shared_ptr<spdlog::sinks::rotating_file_sink<Mutex>>;

auto create_file_sink_tuple(toml::table const &sinktb, bool const &trunct)
        -> std::tuple<toml::string const, bool const>;

template<typename Mutex>
auto create_file_sink_ptr(toml::table const &sinktb, bool const &trunct)
        -> std::shared_ptr<spdlog::sinks::basic_file_sink<Mutex>>;

auto create_daily_file_sink_tuple(toml::table const &sinktb, bool const &trunct, toml::string &&base_filename,
                                  std::size_t const &max_files)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const, bool const, int const>;

template<typename Mutex>
auto create_daily_file_sink_ptr(toml::table const &sinktb, bool const &trunct, toml::string &&base_filename,
                                std::size_t const &max_files) -> std::shared_ptr<spdlog::sinks::daily_file_sink<Mutex>>;

auto create_null_sink_ptr() -> std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>>;

template<typename Mutex>
auto create_stdout_sink_ptr() -> std::shared_ptr<spdlog::sinks::stdout_sink<Mutex>>;

//////// needs work
// template<typename Mutex>
// auto create_stdout_color_sink_ptr() -> std::shared_ptr<spdlog::sinks::stdout_color_sink_st<Mutex>>;

#ifdef __linux__
auto create_syslog_sink_tuple(toml::table const &sinktb)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const, bool const>;

template<typename Mutex>
auto create_syslog_sink_ptr(toml::table const &sinktb) -> std::shared_ptr<spdlog::sinks::syslog_sink<Mutex>>;

#elif _WIN32
template<typename Mutex>
auto create_msvc_sink_ptr() -> std::shared_ptr<spdlog::sinks::msvc_sink<Mutex>>;

#endif

auto gen_from_file_str(toml::string &&typestr, toml::table &&sinktb, bool const &trunct) -> spdlog::sink_ptr;

auto gen_from_rotate_str(toml::string &&typestr, toml::table &&sinktb, toml::string &&base_filename,
                         std::size_t const &max_files) -> spdlog::sink_ptr;

auto gen_from_daily_str(toml::string &&typestr, toml::table &&sinktb, bool const &trunct,
                        toml::string &&base_filename, std::size_t const &max_files) -> spdlog::sink_ptr;

auto gen_from_null_or_std_str(toml::string &&typestr) -> spdlog::sink_ptr;

#ifdef __linux__
auto gen_from_linux_str(toml::string &&typestr, toml::table &&sinktb) -> spdlog::sink_ptr;

#elif _WIN32
static auto gen_from_win_str(toml::string &&typestr) -> spdlog::sink_ptr;

#endif

} // namespace KDSPDSetup::details
