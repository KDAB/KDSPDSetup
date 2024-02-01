/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#pragma once

#include <unordered_map>
#include <map>
#include <vector>

#include <spdlog/async.h>
#include <spdlog/spdlog.h>
#include <spdlog/async_logger.h>
#include <spdlog/pattern_formatter.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/daily_file_sink.h>
#include <spdlog/sinks/null_sink.h>
#include <spdlog/sinks/rotating_file_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/stdout_sinks.h>
#ifdef __linux__
#include <spdlog/sinks/syslog_sink.h>
#elif _WIN32
#include <spdlog/sinks/msvc_sink.h>
#endif

#include <toml.hpp>

/**
 * @brief This namespace contains implementation details and helpers used by functions in
 * KDSPDSetup::setup. This includes lookup for sink types and `spdlog` enums, maps to
 * contain named sinks, patterns, and thread pools (to be used when constructing loggers),
 * and functions that actually construct sink pointers of specific types.
 *
 */
namespace KDSPDSetup::details {

/**
 * @brief Vector of strings of `spdlog` standard output sink typenames. Used when matching a `type` string
 * from a TOML table to determine what additional type-specific fields should be read from the table.
 *
 */
static auto const stdStrs{ std::vector<std::string>{ "stdout_sink_st", "stdout_sink_mt", "stdout_color_sink_st",
                                                     "stdout_color_sink_mt", "color_stdout_sink_st",
                                                     "color_stdout_sink_mt" } };

/**
 * @brief Vector of strings of `spdlog` basic file sink typenames. Used when matching a `type` string
 * from a TOML table to determine what additional type-specific fields should be read from the table.
 *
 */
static auto const fileStrs{ std::vector<std::string>{ "basic_file_sink_st", "basic_file_sink_mt" } };

/**
 * @brief Vector of strings of `spdlog` rotating file sink typenames. Used when matching a `type` string
 * from a TOML table to determine what additional type-specific fields should be read from the table.
 *
 */
static auto const rotateStrs{ std::vector<std::string>{ "rotating_file_sink_st", "rotating_file_sink_mt" } };

/**
 * @brief Vector of strings of `spdlog` daily file sink typenames. Used when matching a `type` string
 * from a TOML table to determine what additional type-specific fields should be read from the table.
 *
 */
static auto const dailyStrs{ std::vector<std::string>{ "daily_file_sink_st", "daily_file_sink_mt" } };

/**
 * @brief Vector of strings of `spdlog` null sink typenames. Used when matching a `type` string
 * from a TOML table to determine what additional type-specific fields should be read from the table.
 *
 */
static auto const nullStrs{ std::vector<std::string>{ "null_sink_st", "null_sink_mt" } };

/**
 * @brief Vector of strings of `spdlog` syslog sink typenames. Used when matching a `type` string
 * from a TOML table to determine what additional type-specific fields should be read from the table.
 *
 */
static auto const linuxStrs{ std::vector<std::string>{ "syslog_sink_st", "syslog_sink_mt" } };

/**
 * @brief Vector of strings of `spdlog` MSVC sink typenames. Used when matching a `type` string
 * from a TOML table to determine what additional type-specific fields should be read from the table.
 *
 */
static auto const winStrs{ std::vector<std::string>{ "msvc_sink_st", "msvc_sink_mt" } };

/**
 * @brief A simple map associating strings of `spdlog::level::level_enum` names to the enums themselves.
 * Used to pass an enum to `spdlog::logger::set_level` given a string read from a TOML table.
 *
 */
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

/**
 * @brief A simple map associating strings of `spdlog::async_overflow_policy` names to the enums themselves.
 * Used to pass an overflow policy to the constructor for an async logger given a string either read from a
 * TOML table or set to the default `"block"`.
 *
 */
static auto const overflowMap{ std::map<toml::string, spdlog::async_overflow_policy>{
        { "overrun_oldest", spdlog::async_overflow_policy::overrun_oldest },
        { "block", spdlog::async_overflow_policy::block },
} };

/**
 * @brief This class contains maps that associate string names to sink pointers, format strings, and
 * thread pool information. This enables simple access of these objects by name when constructing loggers.
 * There are no instances of the class - it is composed entirely of static members.
 *
 */
class SPDMaps
{
public:
    /**
     * @brief Getter method for the private member KDSPDSetup::details::SPDMaps::mSinkMap.
     * Used to index the map and obtain a sink pointer by name when constructing loggers.
     *
     * @return std::map<toml::string, spdlog::sink_ptr> const& A `const` reference to
     * the private data member KDSPDSetup::details::SPDMaps::mSinkMap.
     */
    static auto sinkMap() -> std::map<toml::string, spdlog::sink_ptr> const &;

    /**
     * @brief Getter method for the private member KDSPDSetup::details::SPDMaps::mPatternMap.
     * Used to index the map and obtain a format string by name, to pass into `spdlog::set_pattern`
     * when constructing sinks or loggers.
     *
     * @return std::map<toml::string, toml::string> const& A `const` reference to
     * the private data member KDSPDSetup::details::SPDMaps::mPatternMap.
     */
    static auto patternMap() -> std::map<toml::string, toml::string> const &;

    /**
     * @brief Getter method for the private member KDSPDSetup::details::SPDMaps::mThreadPoolMap.
     * Used to index the map by thread pool name when constructing asynchronous loggers, to obtain
     * a pair containing queue size and thread count.
     *
     * @return std::map<toml::string, std::pair<std::size_t, std::size_t>> const& A `const` reference to
     * the private data member KDSPDSetup::details::SPDMaps::mThreadPoolMap.
     */
    static auto threadPoolMap() -> std::map<toml::string, std::pair<std::size_t, std::size_t>> const &;

    /**
     * @brief Emplace a pair in the private member KDSPDSetup::details::SPDMaps::mSinkMap.
     * This function is called after a sink pointer is created in KDSPDSetup::setup::setupSink,
     * to make it accessible by name when constructing loggers that will use it.
     *
     * @param _pr An rvalue reference pair containing name and a sink pointer, to be emplaced in
     * KDSPDSetup::details::SPDMaps::mSinkMap.
     */
    static void emplaceSinkMap(std::pair<toml::string, spdlog::sink_ptr> &&_pr);

    /**
     * @brief Emplace a pair in the private member KDSPDSetup::details::SPDMaps::mSinkMap.
     * This function is called by KDSPDSetup::setup::setupPatterns when reading a `pattern`
     * table from a configuration file. The pattern string is associated with its name so it
     * can be accessed by name when constructing sinks or loggers that will use the string
     * to format messages.
     *
     * @param _pr An rvalue reference pair containing name and pattern string, to be emplaced
     * in KDSPDSetup::details::SPDMaps::mPatternMap.
     */
    static void emplacePatternMap(std::pair<toml::string, toml::string> &&_pr);

    /**
     * @brief Emplace a pair in the private member KDSPDSetup::details::SPDMaps::mThreadPoolMap.
     * This function is called by KDSPDSetup::setup::setupThreadPools when reading a `thread_pool`
     * table from a configuration file. The thread pool's name is associated with its corresponding
     * queue size and thread count, so they can be used later to construct asynchronous loggers.
     *
     * @param _pr An rvalue reference pair containing name and a nested pair containing a queue size
     * and thread count, to be emplaced in KDSPDSetup::details::SPDMaps::mThreadPoolMap.
     */
    static void emplaceThreadPoolMap(std::pair<toml::string, std::pair<std::size_t, std::size_t>> &&_pr);

private:
    /**
     * @brief Map that associates string names to sink pointers. When sink pointers are created from
     * `sink` tables in a configuration file, they are added here. Then they can be specified by name
     * and looked up from the map when constructing loggers.
     *
     */
    static inline std::map<toml::string, spdlog::sink_ptr> mSinkMap{};

    /**
     * @brief Map that associates string names to pattern strings. When `pattern` tables are read
     * from a configuration file, they are added here. Then they can be specified by name
     * and looked up from the map when constructing loggers and sinks.
     *
     */
    static inline std::map<toml::string, toml::string> mPatternMap{};

    /**
     * @brief Map that associates string names to pairs of thread pool fields. The pairs represent the
     * queue size and thread count of the thread pool, respectively. When `thread_pool` tables are read
     * from a configuration file, their associated fields are added here. Then they can be specified by name,
     * and their corresponding queue size and thread count can be looked up from the map when
     * constructing asynchronous loggers.
     *
     */
    static inline std::map<toml::string, std::pair<std::size_t, std::size_t>> mThreadPoolMap{};
};

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createRotatingFileSinkPtr.
 *
 */
#define createRotatingFileSinkStPtr createRotatingFileSinkPtr<spdlog::details::null_mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createRotatingFileSinkPtr.
 *
 */
#define createRotatingFileSinkMtPtr createRotatingFileSinkPtr<std::mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createFileSinkPtr.
 *
 */
#define createFileSinkStPtr createFileSinkPtr<spdlog::details::null_mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createFileSinkPtr.
 *
 */
#define createFileSinkMtPtr createFileSinkPtr<std::mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createDailyFileSinkPtr.
 *
 */
#define createDailyFileSinkStPtr createDailyFileSinkPtr<spdlog::details::null_mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createDailyFileSinkPtr.
 *
 */
#define createDailyFileSinkMtPtr createDailyFileSinkPtr<std::mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createStdoutSinkPtr.
 *
 */
#define createStdoutSinkStPtr createStdoutSinkPtr<spdlog::details::console_nullmutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createStdoutSinkPtr.
 *
 */
#define createStdoutSinkMtPtr createStdoutSinkPtr<spdlog::details::console_mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createStdoutColorSinkPtr.
 *
 */
#define createStdoutColorSinkStPtr createStdoutColorSinkPtr<spdlog::details::console_nullmutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createStdoutColorSinkPtr.
 *
 */
#define createStdoutColorSinkMtPtr createStdoutColorSinkPtr<spdlog::details::console_mutex>

#ifdef __linux__
/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createSyslogSinkPtr.
 *
 */
#define createSyslogSinkStPtr createSyslogSinkPtr<spdlog::details::null_mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createSyslogSinkPtr.
 *
 */
#define createSyslogSinkMtPtr createSyslogSinkPtr<std::mutex>

#elif _WIN32
/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createMsvcSinkPtr.
 *
 */
#define createMsvcSinkStPtr createMsvcSinkPtr<spdlog::details::null_mutex>

/**
 * @brief A macro alias to improve readability of calls to KDSPDSetup::details::createMsvcSinkPtr.
 *
 */
#define createMsvcSinkMtPtr createMsvcSinkPtr<std::mutex>
#endif

/**
 * @brief Returns true if a string `typeStr` is present in a vector `strList`, and false if not.
 * Used to identify a group to which a sink's `type` belongs when reading from a configuration file.
 * This grouping informs what additional fields should be extracted and narrows down which
 * constructors to call to create a sink.
 *
 * @param typeStr A string of a typename of an spdlog sink.
 * @param strList A vector of typename strings to match against.
 * @return true if `strList` contains `typeStr`.
 * @return false if `strList` does not contain `typeStr`.
 */
bool inTypelist(std::string const &typeStr, std::vector<std::string> const &strList);

/**
 * @brief Generates the argument list for rotating file sink constructor as a tuple and returns it.
 * Given a TOML table representing a rotating file sink, a base filename, and a number of maximum files,
 * extract and calculate the maximum file size from the table, and create a tuple with the base filename,
 * max size, and max number of files in the correct order. Return this tuple so the values can be passed into
 * the constructor for rotating file sink, in that order.
 *
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param baseFilename The base filename to later pass into the constructor for `spdlog::sinks::rotating_file_sink`.
 * @param maxFiles The maximum number of files. This is later passed into the constructor for
 * `spdlog::sinks::rotating_file_sink`.
 * @return std::tuple<toml::string const, std::size_t const, std::size_t const>
 */
auto createRotatingFileSinkTuple(toml::table const &sinkTable, toml::string &&baseFilename,
                                 std::size_t const &maxFiles)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const>;

/**
 * @brief Create a rotating file sink shared pointer and return it. Calls KDSPDSetup::details::createRotatingFileSinkTuple
 * to evaluate the arguments to pass into the constructor, and calls the constructor with those arguments.
 * The macros `createRotatingFileSinkStPtr` and `createRotatingFileSinkMtPtr` exist to improve readability
 * and clarity of intent when calling this function.
 *
 * @tparam Mutex The template parameter to pass to the sink object. Will be either `spdlog::details::null_mutex`
 * or `std::mutex`, for single-threaded or multi-threaded sinks respectively.
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param baseFilename The base filename to pass into the constructor for `spdlog::sinks::rotating_file_sink`.
 * @param maxFiles The maximum number of files. This is passed into the constructor for `spdlog::sinks::rotating_file_sink`.
 * @return std::shared_ptr<spdlog::sinks::rotating_file_sink<Mutex>> A rotating file sink pointer that
 * can be upcast to an `spdlog::sink_ptr` and added to an entry in KDSPDSetup::details::SPDMaps::mSinkMap.
 */
template<typename Mutex>
auto createRotatingFileSinkPtr(toml::table const &sinkTable, toml::string &&baseFilename,
                               std::size_t const &maxFiles)
        -> std::shared_ptr<spdlog::sinks::rotating_file_sink<Mutex>>
{
    auto tup = createRotatingFileSinkTuple(sinkTable, std::move(baseFilename), maxFiles);
    return std::make_shared<spdlog::sinks::rotating_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup),
                                                                      std::get<2>(tup));
}

/**
 * @brief Generates the argument list for basic file sink constructor as a tuple and returns it.
 * Given a TOML table representing a basic file sink and a boolean value for `truncate`, extract the `filename`
 * from the table, and create a tuple with the `filename` and `truncate` in the order in which they will be passed
 * into the basic file sink constructor. Return this tuple.
 *
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param truncate The value of `truncate` to pass into the constructor for basic file sink.
 * @return std::tuple<toml::string const, bool const>
 */
auto createFileSinkTuple(toml::table const &sinkTable, bool const &truncate)
        -> std::tuple<toml::string const, bool const>;

/**
 * @brief Create a basic file sink shared pointer and return it. Calls KDSPDSetup::details::createFileSinkTuple
 * to evaluate the arguments to pass into the constructor, and calls the constructor with those arguments.
 * The macros `createFileSinkStPtr` and `createFileSinkMtPtr` exist to improve readability
 * and clarity of intent when calling this function.
 *
 * @tparam Mutex The template parameter to pass to the sink object. Will be either `spdlog::details::null_mutex`
 * or `std::mutex`, for single-threaded or multi-threaded sinks respectively.
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param truncate The value of `truncate` to pass into the constructor for basic file sink.
 * @return std::shared_ptr<spdlog::sinks::basic_file_sink<Mutex>> A basic file sink pointer that
 * can be upcast to an `spdlog::sink_ptr` and added to an entry in KDSPDSetup::details::SPDMaps::mSinkMap.
 */
template<typename Mutex>
auto createFileSinkPtr(toml::table const &sinkTable, bool const &truncate)
        -> std::shared_ptr<spdlog::sinks::basic_file_sink<Mutex>>
{
    auto tup = createFileSinkTuple(sinkTable, truncate);
    return std::make_shared<spdlog::sinks::basic_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup));
}

/**
 * @brief Generates the argument list for daily file sink constructor as a tuple and returns it.
 * Given a TOML table representing a basic file sink, a base filename, and a number of maximum files,
 * and a boolean value for `truncate`, extract the `rotation_hour` and `rotation_minute` from the
 * table, and create a tuple with the base filename, rotation hour, rotation minute, truncate, and
 * maximum file count in the order in which they will be passed to the basic file sink constructor.
 * Return this tuple.
 *
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param truncate The value of `truncate` to pass into the constructor for daily file sink.
 * @param baseFilename The base filename to pass into the constructor for daily file sink.
 * @param maxFiles The maximum number of files. This is passed into the constructor for daily file sink.
 * @return std::tuple<toml::string const, int const, int const, bool const, uint16_t const>
 */
auto createDailyFileSinkTuple(toml::table &&sinkTable, bool const &truncate, toml::string &&baseFilename,
                              uint16_t const &maxFiles)
        -> std::tuple<toml::string const, int const, int const, bool const, uint16_t const>;

/**
 * @brief Create a daily file sink shared pointer and return it. Calls KDSPDSetup::details::createDailyFileSinkTuple
 * to evaluate the arguments to pass into the constructor, and calls the constructor with those arguments.
 * The macros `createDailyFileSinkStPtr` and `createDailyFileSinkMtPtr` exist to improve readability
 * and clarity of intent when calling this function.
 *
 * @tparam Mutex The template parameter to pass to the sink object. Will be either `spdlog::details::null_mutex`
 * or `std::mutex`, for single-threaded or multi-threaded sinks respectively.
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param truncate The value of `truncate` to pass into the constructor for daily file sink.
 * @param baseFilename The base filename to pass into the constructor for daily file sink.
 * @param maxFiles The maximum number of files. This is passed into the constructor for daily file sink.
 * @return std::shared_ptr<spdlog::sinks::daily_file_sink<Mutex>> A daily file sink pointer that
 * can be upcast to an `spdlog::sink_ptr` and added to an entry in KDSPDSetup::details::SPDMaps::mSinkMap.
 */
template<typename Mutex>
auto createDailyFileSinkPtr(toml::table &&sinkTable, bool const &truncate, toml::string &&baseFilename,
                            uint16_t const &maxFiles) -> std::shared_ptr<spdlog::sinks::daily_file_sink<Mutex>>
{
    auto tup = createDailyFileSinkTuple(std::move(sinkTable), truncate, std::move(baseFilename), maxFiles);
    return std::make_shared<spdlog::sinks::daily_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup),
                                                                   std::get<3>(tup), std::get<4>(tup));
}

/**
 * @brief Create a null sink shared pointer and return it.
 *
 * @return std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>> A null sink pointer that
 * can be upcast to an `spdlog::sink_ptr` and added to an entry in KDSPDSetup::details::SPDMaps::mSinkMap.
 */
auto createNullSinkPtr() -> std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>>;

/**
 * @brief Create a basic file sink shared pointer and return it. The macros `createStdoutSinkStPtr` and
 * `createStdoutSinkMtPtr` exist to improve readability and clarity of intent when calling this function.
 *
 * @tparam Mutex The template parameter to pass to the sink object. Will be either `spdlog::details::console_nullmutex`
 * or `spdlog::details::console_mutex`, for single-threaded or multi-threaded sinks respectively.
 * @return std::shared_ptr<spdlog::sinks::stdout_sink<Mutex>> A standard output sink pointer that
 * can be upcast to an `spdlog::sink_ptr` and added to an entry in KDSPDSetup::details::SPDMaps::mSinkMap.
 */
template<typename Mutex>
auto createStdoutSinkPtr() -> std::shared_ptr<spdlog::sinks::stdout_sink<Mutex>>
{
    return std::make_shared<spdlog::sinks::stdout_sink<Mutex>>();
}

#ifdef _WIN32
/**
 * @brief TODO TODO TODO
 *
 * @tparam Mutex
 * @return std::shared_ptr<spdlog::sinks::wincolor_stdout_sink<Mutex>>
 */
template<typename Mutex>
auto createStdoutColorSinkPtr() -> std::shared_ptr<spdlog::sinks::wincolor_stdout_sink<Mutex>>
{
    return std::make_shared<spdlog::sinks::wincolor_stdout_sink<Mutex>>();
}

#else
/**
 * @brief TODO TODO TODO
 *
 * @tparam Mutex
 * @return std::shared_ptr<spdlog::sinks::ansicolor_stdout_sink<Mutex>>
 */
template<typename Mutex>
auto createStdoutColorSinkPtr() -> std::shared_ptr<spdlog::sinks::ansicolor_stdout_sink<Mutex>>
{
    return std::make_shared<spdlog::sinks::ansicolor_stdout_sink<Mutex>>();
}

#endif

#ifdef __linux__
/**
 * @brief Generates the argument list for syslog sink constructor as a tuple and returns it.
 * Given a TOML table representing a basic file sink, attempt to extract `ident`, `syslog_option`, and
 * `syslog_facility`. Set default values for any of these that aren't found. Create a tuple
 * with the ident, syslog option, syslog facility, and enable formatting (which is always true here).
 * Return the tuple with the values in this order such that they can be passed in the correct order to the
 * constructor for syslog sink.
 *
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @return std::tuple<toml::string const, int const, int const, bool const>
 */
auto createSyslogSinkTuple(toml::table const &sinkTable)
        -> std::tuple<toml::string const, int const, int const, bool const>;

/**
 * @brief Create a syslog sink shared pointer and return it. Calls KDSPDSetup::details::createSyslogSinkTuple
 * to evaluate the arguments to pass into the constructor, and calls the constructor with those arguments.
 * The macros `createSyslogSinkStPtr` and `createSyslogSinkMtPtr` exist to improve readability
 * and clarity of intent when calling this function.
 *
 * @tparam Mutex The template parameter to pass to the sink object. Will be either `spdlog::details::null_mutex`
 * or `std::mutex`, for single-threaded or multi-threaded sinks respectively.
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @return std::shared_ptr<spdlog::sinks::syslog_sink<Mutex>> A syslog sink pointer that
 * can be upcast to an `spdlog::sink_ptr` and added to an entry in KDSPDSetup::details::SPDMaps::mSinkMap.
 */
template<typename Mutex>
auto createSyslogSinkPtr(toml::table &&sinkTable) -> std::shared_ptr<spdlog::sinks::syslog_sink<Mutex>>
{
    auto tup = createSyslogSinkTuple(std::move(sinkTable));
    return std::make_shared<spdlog::sinks::syslog_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup),
                                                               std::get<3>(tup));
}

#elif _WIN32
/**
 * @brief Create an MSVC sink shared pointer and return it. The macros `createMsvcSinkStPtr` and
 * `createMsvcSinkMtPtr` exist to improve readability and clarity of intent when calling this function.
 *
 * @tparam Mutex Mutex The template parameter to pass to the sink object. Will be either `spdlog::details::null_mutex`
 * or `std::mutex`, for single-threaded or multi-threaded sinks respectively.
 * @return std::shared_ptr<spdlog::sinks::msvc_sink<Mutex>> An MSVC sink pointer that can be upcast to an
 * `spdlog::sink_ptr` and added to an entry in KDSPDSetup::details::SPDMaps::mSinkMap.
 */
template<typename Mutex>
auto createMsvcSinkPtr() -> std::shared_ptr<spdlog::sinks::msvc_sink<Mutex>>
{
    return std::make_shared<spdlog::sinks::msvc_sink<Mutex>>();
}
#endif

/**
 * @brief Return the result of calling KDSPDSetup::details::createFileSinkPtr with the correct template argument
 * based on the value of `typeStr`. Uses macros `createFileSinkStPtr` and `createFileSinkMtPtr` for clarity.
 * This function is called by KDSPDSetup::setup::handleTruncatableSink.
 *
 * @param typeStr A string of a typename of an spdlog sink.
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param truncate The value of `truncate` to pass into the constructor for basic file sink.
 * @return spdlog::sink_ptr The result of calling KDSPDSetup::details::createFileSinkPtr.
 */
auto genFromFileStr(toml::string &&typeStr, toml::table &&sinkTable, bool const &truncate) -> spdlog::sink_ptr;

/**
 * @brief Return the result of calling KDSPDSetup::details::createRotatingFileSinkPtr with the correct template argument
 * based on the value of `typeStr`. Uses macros `createRotatingFileSinkStPtr` and `createRotatingFileSinkMtPtr` for clarity.
 * This function is called by KDSPDSetup::setup::handleMultipleFileSink.
 *
 * @param typeStr A string of a typename of an spdlog sink.
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param baseFilename The base filename to pass into the constructor for rotating file sink.
 * @param maxFiles The maximum number of files. This is passed into the constructor for rotating file sink.
 * @return spdlog::sink_ptr The result of calling KDSPDSetup::details::createRotatingFileSinkPtr.
 */
auto genFromRotateStr(toml::string &&typeStr, toml::table &&sinkTable, toml::string &&baseFilename,
                      std::size_t const &maxFiles) -> spdlog::sink_ptr;

/**
 * @brief Return the result of calling KDSPDSetup::details::createDailyFileSinkPtr with the correct template argument
 * based on the value of `typeStr`. Uses macros `createDailyFileSinkStPtr` and `createDailyFileSinkMtPtr` for clarity.
 * This function is called by KDSPDSetup::setup::handleMultipleFileSink.
 *
 * @param typeStr A string of a typename of an spdlog sink.
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param truncate The value of `truncate` to pass into the constructor for daily file sink.
 * @param baseFilename The base filename to pass into the constructor for daily file sink.
 * @param maxFiles The maximum number of files. This is passed into the constructor for daily file sink.
 * @return spdlog::sink_ptr The result of calling KDSPDSetup::details::createDailyFileSinkPtr.
 */
auto genFromDailyStr(toml::string &&typeStr, toml::table &&sinkTable, bool const &truncate,
                     toml::string &&baseFilename, uint16_t const &maxFiles) -> spdlog::sink_ptr;

/**
 * @brief Return the result of calling KDSPDSetup::details::createStdoutSinkPtr with the correct template argument or
 * KDSPDSetup::details::createNullSinkPtr, based on the value of `typeStr`. Uses macros `createStdoutSinkStPtr`
 * and `createStdoutSinkMtPtr` for clarity.
 * This function is called by KDSPDSetup::setup::setupSink.
 *
 * @param typeStr A string of a typename of an spdlog sink.
 * @return spdlog::sink_ptr The result of calling KDSPDSetup::details::createStdoutSinkPtr or KDSPDSetup::details::createNullSinkPtr.
 */
auto genFromNullOrStdStr(toml::string &&typeStr) -> spdlog::sink_ptr;

#ifdef __linux__
/**
 * @brief Return the result of calling KDSPDSetup::details::createSyslogSinkPtr with the correct template argument
 * based on the value of `typeStr`. Uses macros `createSyslogSinkStPtr` and `createSyslogSinkMtPtr` for clarity.
 * This function is called by KDSPDSetup::setup::setupSink.
 *
 * @param typeStr A string of a typename of an spdlog sink.
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @return spdlog::sink_ptr The result of calling KDSPDSetup::details::createSyslogSinkPtr.
 */
auto genFromLinuxStr(toml::string &&typeStr, toml::table &&sinkTable) -> spdlog::sink_ptr;

#elif _WIN32
/**
 * @brief Return the result of calling KDSPDSetup::details::createMsvcSinkPtr with the correct template argument
 * based on the value of `typeStr`. Uses macros `createMsvcSinkStPtr` and `createMsvcSinkMtPtr` for clarity.
 * This function is called by KDSPDSetup::setup::setupSink.
 *
 * @param typeStr A string of a typename of an spdlog sink.
 * @return spdlog::sink_ptr The result of calling KDSPDSetup::details::createMsvcSinkPtr.
 */
auto genFromWinStr(toml::string &&typeStr) -> spdlog::sink_ptr;

#endif

} // namespace KDSPDSetup::details
