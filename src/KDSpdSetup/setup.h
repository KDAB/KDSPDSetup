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
 * of these types of sinks. Based on the type specified in typeStr, either KDSPDSetup::details::genFromRotateStr
 * or KDSPDSetup::details::genFromDailyStr are invoked with these values. This function is called by
 * KDSPDSetup::setup::handleTruncatableSink when `typeStr` specifies a rotating or daily file sink.
 * 
 * @param typeStr String of the sink's type, which is checked against string vectors in KDSPDSetup::details
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param sinkPtr An uninitialized sink pointer, which will be passed to functions that construct a sink.
 * This pointer is eventually put into KDSPDSetup::details::SPDMaps::mSinkMap at the bottom of 
 * KDSPDSetup::setup::setupSink.
 * @param truncate The value for `truncate` obtained in caller KDSPDSetup::setup::handleTruncatableSink.
 */
void handleMultipleFileSink(toml::string &&typeStr, toml::table &&sinkTable, spdlog::sink_ptr &sinkPtr, bool const &truncate);

/**
 * @brief Helper function for setting up file-based sinks that require a `truncate` argument in 
 * their constructor. Further narrows down the type after determining value of `truncate` and
 * calls either KDSPDSetup::details::genFromFileStr or KDSPDSetup::setup::handleMultipleFileSink.
 * This function is invoked by KDSPDSetup::setup::setupSink.
 * 
 * @param typeStr String of the sink's type, which is checked against string vectors in KDSPDSetup::details
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * @param sinkPtr An uninitialized sink pointer, which will be passed to functions that construct a sink.
 * This pointer is eventually put into KDSPDSetup::details::SPDMaps::mSinkMap at the bottom of 
 * KDSPDSetup::setup::setupSink.
 */
void handleTruncatableSink(toml::string &&typeStr, toml::table &&sinkTable, spdlog::sink_ptr &sinkPtr);

/**
 * @brief Given a table representation of a sink, creates an `spdlog::sink_ptr`, and evaluates the
 * type of sink specified in the table. Based on the type, calls additional functions to 
 * parse type-specific fields and construct the sink with the correct constructor.
 * Adds the resultant sink to KDSPDSetup::details::SPDMaps::mSinkMap, which is used to obtain
 * sink pointers by name when constructing loggers.
 * 
 * @param sinkTable toml11 table representation for a particular sink specified in a configuration file.
 * Passed in by KDSPDSetup::setup::setupSinks.
 */
void setupSink(toml::table &&sinkTable);

/**
 * @brief Using data read from a toml file, extract vector of tables representing every sink specified.
 * Call KDSPDSetup::setup::setupSink on each table in the vector to create an spdlog sink for each.
 * These sinks are put into KDSPDSetup::details::SPDMaps::mSinkMap, which is later indexed when
 * constructing loggers to obtain the named sink pointers.
 * 
 * @param data toml11 representation of the configuration file.
 */
void setupSinks(toml::value const &data);

/**
 * @brief Using data read from a toml file, search for the key `global_pattern`. If it exists,
 * use its string value as the global pattern for spdlog by passing it to `spdlog::set_pattern`.
 * Search again for a table called pattern. If found, extract vector of tables representing every
 * pattern specified. For each table in the vector, add the name and pattern format string
 * specified to KDSPDSetup::details::SPDMaps::mPatternMap. This map is used when constructing loggers
 * that will use non-global patterns to format messages, allowing indexing of format strings
 * by name.
 * 
 * @param data toml11 representation of the configuration file.
 */
void setupPatterns(toml::value const &data);

/**
 * @brief Using data read from a toml file, search for a table `global_thread_pool`, and initialize
 * it with the specified queue size and thread count fields by calling `spdlog::init_thread_pool`.
 * Search again for a table `thread_pool`. If found, extract vector of tables representing every
 * threadpool specified. For each table in the vector, add the name, queue size, and thread count
 * specified to KDSPDSetup::details::SPDMaps::mThreadPoolMap. This map is used to index threadpool 
 * information by name when constructing asynchronous loggers.
 * 
 * @param data toml11 representation of the configuration file.
 */
void setupThreadPools(toml::value const &data);

/**
 * @brief Creates an `std::shared_ptr<spdlog::async_logger>` initialized with `name`, the sink pointers
 * in `sinklist`, a thread pool pointer, and an overflow policy. The thread pool pointer is constructed
 * with values obtained by indexing KDSPDSetup::details::SPDMaps::mThreadPoolMap by the specified `thread_pool`
 * name in `loggerTable`. If not specified, the thread pool is set to `spdlog::thread_pool()`. The
 * overflow policy is searched for in `loggerTable`, and defaults to `"block"` if not found.
 * The function also sets a formatting pattern if `pattern` is nonempty before registering
 * the logger by calling `spdlog::register_logger`.
 * 
 * @param loggerTable toml11 table representation for a particular logger specified in a configuration file.
 * @param name A string with the name of the logger. This name is used to construct the logger and access 
 * it with `spdlog::get`.
 * @param sinkList A vector of `spdlog::sink_ptr` objects to use as the sinks for the logger.
 * @param pattern The format string for messages the logger will output. If empty, the logger uses the 
 * global pattern set up earlier in KDSPDSetup::setup::setupPatterns, or spdlog's default pattern if
 * the global pattern was not created.
 */
void registerAsynchronousLogger(toml::table const &loggerTable, toml::string const &name, std::vector<spdlog::sink_ptr> const &sinkList,
                                toml::string const &pattern);

/**
 * @brief Creates an `std::shared_ptr<spdlog::logger>` initialized with `name` and the sink pointers
 * in `sinklist`, sets the logger's `pattern` if nonempty and `level` if specified in `loggerTable`,
 * and registers the logger by calling `spdlog::register_logger` on the `shared_ptr`.
 * 
 * @param loggerTable toml11 table representation for a particular logger specified in a configuration file.
 * @param name A string with the name of the logger. This name is used to construct the logger and access 
 * it with `spdlog::get`.
 * @param sinkList A vector of `spdlog::sink_ptr` objects to use as the sinks for the logger.
 * @param pattern The format string for messages the logger will output. If empty, the logger uses the 
 * global pattern set up earlier in KDSPDSetup::setup::setupPatterns, or spdlog's default pattern if
 * the global pattern was not created.
 */
void registerSynchronousLogger(toml::table const &loggerTable, toml::string const &name, std::vector<spdlog::sink_ptr> const &sinkList,
                               toml::string const &pattern);

/**
 * @brief Given a table representation of a logger whose name is not already registered with spdlog,
 * evaluates whether the logger's specified type is asynchronous or not, and calls either
 * KDSPDSetup::setup::registerAsynchronousLogger or KDSPDSetup::setup::registerSynchronousLogger accordingly.
 * These functions are called with `loggerTable`, the specified name, a list of sink pointers, and the
 * specified pattern, if found. The list of sink pointers is obtained by indexing
 * KDSPDSetup::details::SPDMaps::mSinkMap with names specified in a list in the `loggerTable`.
 * 
 * @param loggerTable toml11 table representation for a particular logger specified in a configuration file.
 * Passed in by KDSPDSetup::setup::setupLoggers.
 */
void setupLogger(toml::table const &loggerTable);

/**
 * @brief Using data read from a toml file, extract vector of tables representing every logger specified.
 * Call KDSPDSetup::setup::setupLogger on each table in the vector to create and register an spdlog
 * logger for each. The loggers should then be available through `spdlog::get`.
 * 
 * @param data toml11 representation of the configuration file.
 */
void setupLoggers(toml::value const &data);

} // namespace KDSPDSetup::setup
