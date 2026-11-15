/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#pragma once

#include "details.h"

/**
 * @brief This namespace contains the functions directly called by KDSPDSetup::setupFrom, along with
 * helpers called by those functions. The file which contains the definitions for members of 
 * KDSPDSetup::setup, setup.h, is included in the implementation file kdspdsetup.cpp
 * The functions defined in this namespace make use of the members of namespace KDSPDSetup::details,
 * defined in details.h
 * 
 */
namespace KDSPDSetup::setup {

/**
 * @brief Helper function for setting up sinks that use multiple files. The function extracts
 * values for `base_filename` and `max_files` from the given table, which are needed by the constructors
 * of these types of sinks. Based on `sink.type`, either KDSPDSetup::details::genFromRotateStr
 * or KDSPDSetup::details::genFromDailyStr are invoked with these values. This function is called by
 * KDSPDSetup::setup::handleTruncatableSink when `sink.type` specifies a rotating or daily file sink.
 * 
 * @param sink Typed representation of a particular sink specified in a configuration file.
 * @param sinkPtr An uninitialized sink pointer, which will be passed to functions that construct a sink.
 * This pointer is eventually put into KDSPDSetup::details::SPDMaps::mSinkMap at the bottom of 
 * KDSPDSetup::setup::setupSink.
 */
void handleMultipleFileSink(SinkConfig const &sink, spdlog::sink_ptr &sinkPtr);

/**
 * @brief Helper function for setting up file-based sinks that require a `truncate` argument in 
 * their constructor. Further narrows down the type after determining value of `truncate` and
 * calls either KDSPDSetup::details::genFromFileStr or KDSPDSetup::setup::handleMultipleFileSink.
 * This function is invoked by KDSPDSetup::setup::setupSink.
 * 
 * @param sink Typed representation of a particular sink specified in a configuration file.
 * @param sinkPtr An uninitialized sink pointer, which will be passed to functions that construct a sink.
 * This pointer is eventually put into KDSPDSetup::details::SPDMaps::mSinkMap at the bottom of 
 * KDSPDSetup::setup::setupSink.
 */
void handleTruncatableSink(SinkConfig const &sink, spdlog::sink_ptr &sinkPtr);

/**
 * @brief Given the configuration of a sink, creates an `spdlog::sink_ptr`, and evaluates the
 * type of sink specified in the configuration. Based on the type, calls additional functions to 
 * parse type-specific fields and construct the sink with the correct constructor.
 * Adds the resultant sink to KDSPDSetup::details::SPDMaps::mSinkMap, which is used to obtain
 * sink pointers by name when constructing loggers.
 * 
 * @param sink Typed representation of a particular sink specified in a configuration file.
 * Passed in by KDSPDSetup::setup::setupSinks.
 */
void setupSink(SinkConfig const &sink);

/**
 * @brief Using the parsed configuration, create every sink specified.
 * Call KDSPDSetup::setup::setupSink on each entry to create an spdlog sink for each.
 * These sinks are put into KDSPDSetup::details::SPDMaps::mSinkMap, which is later indexed when
 * constructing loggers to obtain the named sink pointers.
 * 
 * @param config The parsed configuration.
 */
void setupSinks(Configuration const &config);

/**
 * @brief Using the parsed configuration, search for the key `global_pattern`. If it exists,
 * use its string value as the global pattern for spdlog by passing it to `spdlog::set_pattern`.
 * Search again for a table called pattern. If found, create every
 * pattern specified. For each entry, add the name and pattern format string
 * specified to KDSPDSetup::details::SPDMaps::mPatternMap. This map is used when constructing loggers
 * that will use non-global patterns to format messages, allowing indexing of format strings
 * by name.
 * 
 * @param config The parsed configuration.
 */
void setupPatterns(Configuration const &config);

/**
 * @brief Using the parsed configuration, search for a table `global_thread_pool`, and initialize
 * it with the specified queue size and thread count fields by calling `spdlog::init_thread_pool`.
 * Search again for a table `thread_pool`. If found, create every
 * threadpool specified. For each entry, add the name, queue size, and thread count
 * specified to KDSPDSetup::details::SPDMaps::mThreadPoolMap. This map is used to index threadpool 
 * information by name when constructing asynchronous loggers.
 * 
 * @param config The parsed configuration.
 */
void setupThreadPools(Configuration const &config);

/**
 * @brief Creates an `std::shared_ptr<spdlog::async_logger>` initialized with `name`, the sink pointers
 * in `sinklist`, a thread pool pointer, and an overflow policy. The thread pool pointer is constructed
 * with values obtained by indexing KDSPDSetup::details::SPDMaps::mThreadPoolMap by the specified `thread_pool`
 * name in `logger.thread_pool`. If not specified, the thread pool is set to `spdlog::thread_pool()`. The
 * overflow policy is read from `logger.overflow_policy`, and defaults to `"block"` if not found.
 * The function also sets a formatting pattern if `pattern` is nonempty before registering
 * the logger by calling `spdlog::register_logger`.
 * 
 * @param logger Typed representation of a particular logger specified in a configuration file.
 * @param sinkList A vector of `spdlog::sink_ptr` objects to use as the sinks for the logger.
 * @param pattern The format string for messages the logger will output. If empty, the logger uses the 
 * global pattern set up earlier in KDSPDSetup::setup::setupPatterns, or spdlog's default pattern if
 * the global pattern was not created.
 */
void registerAsynchronousLogger(LoggerConfig const &logger, std::vector<spdlog::sink_ptr> const &sinkList,
                                std::string const &pattern);

/**
 * @brief Creates an `std::shared_ptr<spdlog::logger>` initialized with `name` and the sink pointers
 * in `sinklist`, sets the logger's `pattern` if nonempty and `level` if specified in `logger`,
 * and registers the logger by calling `spdlog::register_logger` on the `shared_ptr`.
 * 
 * @param logger Typed representation of a particular logger specified in a configuration file.
 * @param sinkList A vector of `spdlog::sink_ptr` objects to use as the sinks for the logger.
 * @param pattern The format string for messages the logger will output. If empty, the logger uses the 
 * global pattern set up earlier in KDSPDSetup::setup::setupPatterns, or spdlog's default pattern if
 * the global pattern was not created.
 */
void registerSynchronousLogger(LoggerConfig const &logger, std::vector<spdlog::sink_ptr> const &sinkList,
                               std::string const &pattern);

/**
 * @brief Given the configuration of a logger whose name is not already registered with spdlog,
 * evaluates whether the logger's specified type is asynchronous or not, and calls either
 * KDSPDSetup::setup::registerAsynchronousLogger or KDSPDSetup::setup::registerSynchronousLogger accordingly.
 * These functions are called with `logger`, a list of sink pointers, and the
 * specified pattern, if found. The list of sink pointers is obtained by indexing
 * KDSPDSetup::details::SPDMaps::mSinkMap with the names listed in `logger.sinks`.
 * 
 * @param logger Typed representation of a particular logger specified in a configuration file.
 * Passed in by KDSPDSetup::setup::setupLoggers.
 */
void setupLogger(LoggerConfig const &logger);

/**
 * @brief Using the parsed configuration, create every logger specified.
 * Call KDSPDSetup::setup::setupLogger on each entry to create and register an spdlog
 * logger for each. The loggers should then be available through `spdlog::get`.
 * 
 * @param config The parsed configuration.
 */
void setupLoggers(Configuration const &config);

} // namespace KDSPDSetup::setup
