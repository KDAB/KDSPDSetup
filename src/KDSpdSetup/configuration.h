/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#pragma once

#include <cstddef>
#include <optional>
#include <string>
#include <string_view>
#include <vector>

#include <rfl.hpp>

/**
 * @brief The configuration model of the library, plus the functions that read it
 * from disk using reflect-cpp.
 *
 * The structs below mirror the layout of a configuration file one to one: every
 * member is named exactly like the key it is read from. They are plain aggregates,
 * so they can be built with designated initializers in tests and in user code:
 *
 * @code
 * KDSPDSetup::SinkConfig sink{ .name = "console", .type = "stdout_sink_mt" };
 * @endcode
 *
 * Because the members are plain aggregates, reflect-cpp only needs the member names
 * to map a document onto them. Fields that a configuration file may omit are either
 * `std::optional` (when "absent" carries meaning) or carry a default value and are
 * filled in by the `rfl::DefaultIfMissing` processor.
 */
namespace KDSPDSetup {

/**
 * @brief A single sink as described by an entry of the `sink` array of tables.
 *
 * Only `name` and `type` are required for every sink. Which of the remaining fields
 * are required depends on `type`; KDSPDSetup::configuration::validate enforces that.
 */
struct SinkConfig {
    /// @brief Name the sink is registered under, so loggers can refer to it.
    std::string name;

    /// @brief spdlog sink type, e.g. `stdout_sink_mt` or `rotating_file_sink_st`.
    std::string type;

    /// @brief Optional log level of the sink.
    std::optional<std::string> level;

    /// @brief Required by `basic_file_sink_st` and `basic_file_sink_mt`.
    std::optional<std::string> filename;

    /// @brief Truncate an existing file on start-up. Defaults to `false`.
    bool truncate = false;

    /// @brief Required by the rotating and daily file sinks.
    std::optional<std::string> base_filename;

    /// @brief Required by the rotating file sinks, e.g. `1M`. Suffixes `K`..`T` are supported.
    std::optional<std::string> max_size;

    /// @brief Number of files to rotate over. Defaults to `0`, meaning spdlog's default.
    std::size_t max_files = 0;

    /// @brief Hour of the day at which a daily file sink rotates. Defaults to `0`.
    int rotation_hour = 0;

    /// @brief Minute of the day at which a daily file sink rotates. Defaults to `0`.
    int rotation_minute = 0;

    /// @brief Identifier passed to syslog. Syslog sinks only.
    std::optional<std::string> ident;

    /// @brief Logging option passed to syslog. Syslog sinks only.
    std::optional<int> syslog_option;

    /// @brief Syslog facility. Syslog sinks only; falls back to `LOG_USER`.
    std::optional<int> syslog_facility;
};

/**
 * @brief A named format string, as described by an entry of the `pattern` array of tables.
 */
struct PatternConfig {
    /// @brief Name the pattern is registered under, so loggers can refer to it.
    std::string name;

    /// @brief The spdlog format string.
    std::string value;
};

/**
 * @brief A thread pool, as described by the `global_thread_pool` table or by an entry
 * of the `thread_pool` array of tables.
 */
struct ThreadPoolConfig {
    /// @brief Name the pool is registered under. Unused for the global thread pool.
    std::string name;

    /// @brief Size of the queue handed to `spdlog::init_thread_pool`.
    std::size_t queue_size = 0;

    /// @brief Number of worker threads handed to `spdlog::init_thread_pool`.
    std::size_t num_threads = 0;
};

/**
 * @brief A single logger, as described by an entry of the `logger` array of tables.
 */
struct LoggerConfig {
    /// @brief Name the logger is registered under. Required.
    std::string name;

    /// @brief `async` creates an asynchronous logger; anything else creates a synchronous one.
    std::string type;

    /// @brief Names of the sinks this logger writes to. Required.
    std::vector<std::string> sinks;

    /// @brief Optional log level of the logger.
    std::optional<std::string> level;

    /// @brief Name of a pattern declared in the `pattern` array. Overrides `global_pattern`.
    std::optional<std::string> pattern;

    /// @brief Name of a thread pool declared in the `thread_pool` array. Asynchronous loggers only.
    std::optional<std::string> thread_pool;

    /// @brief `block` (default) or `overrun_oldest`. Asynchronous loggers only.
    std::optional<std::string> overflow_policy;
};

/**
 * @brief A complete configuration document.
 */
struct Configuration {
    /// @brief Format string applied to sinks and loggers that do not name their own pattern.
    std::optional<std::string> global_pattern;

    /// @brief Every sink to create, from the `[[sink]]` array of tables.
    std::vector<SinkConfig> sink;

    /// @brief Every named pattern, from the `[[pattern]]` array of tables.
    std::vector<PatternConfig> pattern;

    /// @brief The thread pool handed to `spdlog::init_thread_pool`, if any.
    std::optional<ThreadPoolConfig> global_thread_pool;

    /// @brief Every named thread pool, from the `[[thread_pool]]` array of tables.
    std::vector<ThreadPoolConfig> thread_pool;

    /// @brief Every logger to create, from the `[[logger]]` array of tables.
    std::vector<LoggerConfig> logger;
};

/**
 * @brief This namespace contains everything that turns a configuration file into
 * KDSPDSetup::Configuration. The parsing itself is done by reflect-cpp.
 */
namespace configuration {

/**
 * @brief The serialization formats a configuration file can be written in.
 */
enum class Format {
    toml,
    json,
    yaml,
};

/**
 * @brief Derive the format of a configuration file from its file name extension.
 *
 * Recognized extensions are `.toml`, `.json`, `.yaml` and `.yml`.
 *
 * @param path Path or file name of the configuration file.
 * @return std::optional<Format> The matching format, or `std::nullopt` if the extension
 * is not recognized.
 */
auto formatFromExtension(std::string_view path) -> std::optional<Format>;

/**
 * @brief Check that the document is complete enough to be applied.
 *
 * reflect-cpp already rejects documents that do not match the model at all, so what is
 * left here are the semantic rules that depend on the value of another field: which
 * fields a given sink type needs, and which names must not be empty.
 *
 * @param config The configuration to check.
 * @throws std::runtime_error With a message naming the offending entry.
 */
void validate(Configuration const &config);

/**
 * @brief Parse a configuration document that is already in memory.
 *
 * @param content The document itself.
 * @param format The format `content` is written in.
 * @return Configuration The parsed and validated configuration.
 * @throws std::runtime_error If reflect-cpp cannot parse `content`, or if the result does
 * not pass KDSPDSetup::configuration::validate.
 */
auto loadFromString(std::string_view content, Format format) -> Configuration;

/**
 * @brief Read and parse a configuration file.
 *
 * The format is derived from the extension of `path`, see
 * KDSPDSetup::configuration::formatFromExtension.
 *
 * @param path Path to the configuration file.
 * @return Configuration The parsed and validated configuration.
 * @throws std::invalid_argument If the file extension is not recognized.
 * @throws std::runtime_error If the file cannot be read, cannot be parsed, or does not
 * pass KDSPDSetup::configuration::validate.
 */
auto load(std::string const &path) -> Configuration;

/**
 * @brief Read and parse a configuration file in an explicitly given format.
 *
 * @param path Path to the configuration file.
 * @param format The format the file is written in.
 * @return Configuration The parsed and validated configuration.
 * @throws std::runtime_error If the file cannot be read, cannot be parsed, or does not
 * pass KDSPDSetup::configuration::validate.
 */
auto load(std::string const &path, Format format) -> Configuration;

} // namespace configuration

} // namespace KDSPDSetup
