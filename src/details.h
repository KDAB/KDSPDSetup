#pragma once

#include "includes.h"

namespace KDSPDSetup::details {

static auto const stdStrs{ std::vector<std::string>{ "stdout_sink_st", "stdout_sink_mt", "stdout_color_sink_st",
                                                     "stdout_color_sink_mt", "color_stdout_sink_st",
                                                     "color_stdout_sink_mt" } };
static auto const fileStrs{ std::vector<std::string>{ "basic_file_sink_st", "basic_file_sink_mt" } };
static auto const rotateStrs{ std::vector<std::string>{ "rotating_file_sink_st", "rotating_file_sink_mt" } };
static auto const dailyStrs{ std::vector<std::string>{ "daily_file_sink_st", "daily_file_sink_mt" } };
static auto const nullStrs{ std::vector<std::string>{ "null_sink_st", "null_sink_mt" } };
static auto const linuxStrs{ std::vector<std::string>{ "syslog_sink_st", "syslog_sink_mt" } };
static auto const winStrs{ std::vector<std::string>{ "msvc_sink_st", "msvc_sink_mt" } };

static auto const levelMap{ std::unordered_map<std::string, spdlog::level::level_enum>{
        { "critical", spdlog::level::level_enum::critical },
        { "debug", spdlog::level::level_enum::debug },
        { "err", spdlog::level::level_enum::err },
        { "info", spdlog::level::level_enum::info },
        { "n_levels", spdlog::level::level_enum::n_levels },
        { "off", spdlog::level::level_enum::off },
        { "trace", spdlog::level::level_enum::trace },
        { "warn", spdlog::level::level_enum::warn },
} };

static auto const overflowMap{ std::map<toml::string, spdlog::async_overflow_policy>{
        { "overrun_oldest", spdlog::async_overflow_policy::overrun_oldest },
        { "block", spdlog::async_overflow_policy::block },
} };

class SPDMaps
{
public:
    static auto sinkMap() -> std::map<toml::string, spdlog::sink_ptr> const &;
    static auto patternMap() -> std::map<toml::string, toml::string> const &;
    static auto threadPoolMap() -> std::map<toml::string, std::pair<std::size_t, std::size_t>> const &;
    static void emplaceSinkMap(std::pair<toml::string, spdlog::sink_ptr> &&_pr);
    static void emplacePatternMap(std::pair<toml::string, toml::string> &&_pr);
    static void emplaceThreadPoolMap(std::pair<toml::string, std::pair<std::size_t, std::size_t>> &&_pr);

private:
    static inline std::map<toml::string, spdlog::sink_ptr> mSinkMap{};
    static inline std::map<toml::string, toml::string> mPatternMap{};
    static inline std::map<toml::string, std::pair<std::size_t, std::size_t>> mThreadPoolMap{};
};

#define createRotatingFileSinkStPtr createRotatingFileSinkPtr<spdlog::details::null_mutex>
#define createRotatingFileSinkMtPtr createRotatingFileSinkPtr<std::mutex>

#define createFileSinkStPtr createFileSinkPtr<spdlog::details::null_mutex>
#define createFileSinkMtPtr createFileSinkPtr<std::mutex>

#define createDailyFileSinkStPtr createDailyFileSinkPtr<spdlog::details::null_mutex>
#define createDailyFileSinkMtPtr createDailyFileSinkPtr<std::mutex>

#define createStdoutSinkStPtr createStdoutSinkPtr<spdlog::details::console_nullmutex>
#define createStdoutSinkMtPtr createStdoutSinkPtr<spdlog::details::console_mutex>

#ifdef __linux__
#define createSyslogSinkStPtr createSyslogSinkPtr<spdlog::details::null_mutex>
#define createSyslogSinkMtPtr createSyslogSinkPtr<std::mutex>
#elif _WIN32
#define createMsvcSinkStPtr createMsvcSinkPtr<spdlog::details::null_mutex>
#define createMsvcSinkMtPtr createMsvcSinkPtr<std::mutex>
#endif

bool inTypelist(std::string const &typeStr, std::vector<std::string> const &strList);

auto createRotatingFileSinkTuple(toml::table const &sinkTable, toml::string &&baseFilename,
                                 std::size_t const &maxFiles)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const>;

template<typename Mutex>
auto createRotatingFileSinkPtr(toml::table const &sinkTable, toml::string &&baseFilename,
                               std::size_t const &maxFiles)
        -> std::shared_ptr<spdlog::sinks::rotating_file_sink<Mutex>>;

auto createFileSinkTuple(toml::table const &sinkTable, bool const &trunct)
        -> std::tuple<toml::string const, bool const>;

template<typename Mutex>
auto createFileSinkPtr(toml::table const &sinkTable, bool const &trunct)
        -> std::shared_ptr<spdlog::sinks::basic_file_sink<Mutex>>;

auto createDailyFileSinkTuple(toml::table &&sinkTable, bool const &trunct, toml::string &&baseFilename,
                              uint16_t const &maxFiles)
        -> std::tuple<toml::string const, int const, int const, bool const, uint16_t const>;

template<typename Mutex>
auto createDailyFileSinkPtr(toml::table &&sinkTable, bool const &trunct, toml::string &&baseFilename,
                            uint16_t const &maxFiles) -> std::shared_ptr<spdlog::sinks::daily_file_sink<Mutex>>;

auto createNullSinkPtr() -> std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>>;

template<typename Mutex>
auto createStdoutSinkPtr() -> std::shared_ptr<spdlog::sinks::stdout_sink<Mutex>>;

//////// needs work
// template<typename Mutex>
// auto create_stdout_color_sink_ptr() -> std::shared_ptr<spdlog::sinks::stdout_color_sink_st<Mutex>>;

#ifdef __linux__
auto createSyslogSinkTuple(toml::table const &sinkTable)
        -> std::tuple<toml::string const, int const, int const, bool const>;

template<typename Mutex>
auto createSyslogSinkPtr(toml::table &&sinkTable) -> std::shared_ptr<spdlog::sinks::syslog_sink<Mutex>>;
#elif _WIN32
template<typename Mutex>
auto createMsvcSinkPtr() -> std::shared_ptr<spdlog::sinks::msvc_sink<Mutex>>;
#endif

auto genFromFileStr(toml::string &&typeStr, toml::table &&sinkTable, bool const &trunct) -> spdlog::sink_ptr;

auto genFromRotateStr(toml::string &&typeStr, toml::table &&sinkTable, toml::string &&baseFilename,
                      std::size_t const &maxFiles) -> spdlog::sink_ptr;

auto genFromDailyStr(toml::string &&typeStr, toml::table &&sinkTable, bool const &trunct,
                     toml::string &&baseFilename, uint16_t const &maxFiles) -> spdlog::sink_ptr;

auto genFromNullOrStdStr(toml::string &&typeStr) -> spdlog::sink_ptr;

#ifdef __linux__
auto genFromLinuxStr(toml::string &&typeStr, toml::table &&sinkTable) -> spdlog::sink_ptr;

#elif _WIN32
auto genFromWinStr(toml::string &&typeStr) -> spdlog::sink_ptr;

#endif

} // namespace KDSPDSetup::details
