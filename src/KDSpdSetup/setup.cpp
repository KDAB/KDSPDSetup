/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "setup.h"

#include <iostream>

using std::literals::string_literals::operator""s;

namespace KDSPDSetup::setup {

void handleMultipleFileSink(SinkConfig const &sink, spdlog::sink_ptr &sinkPtr)
{
    if (details::inTypelist(sink.type, details::rotateStrs)) {
        sinkPtr = details::genFromRotateStr(sink);
    }

    else if (details::inTypelist(sink.type, details::dailyStrs)) {
        sinkPtr = details::genFromDailyStr(sink);
    }
}

void handleTruncatableSink(SinkConfig const &sink, spdlog::sink_ptr &sinkPtr)
{
    if (details::inTypelist(sink.type, details::fileStrs)) {
        sinkPtr = details::genFromFileStr(sink);
    } else if (details::inTypelist(sink.type, details::rotateStrs) || details::inTypelist(sink.type, details::dailyStrs)) {
        handleMultipleFileSink(sink, sinkPtr);
    }
}

void setupSink(SinkConfig const &sink)
{
    spdlog::sink_ptr sinkPtr;

    auto const &name = sink.name;
    auto const &type = sink.type;

    bool ok = false;
    for (const auto &typeList : { details::fileStrs, details::rotateStrs, details::dailyStrs, details::nullStrs, details::stdStrs, details::linuxStrs, details::winStrs }) {
        if (details::inTypelist(type, typeList)) {
            ok = true;
            break;
        }
    }

    if (!ok)
        throw std::out_of_range("KDSPDSetup: sink type "s + type + " is not valid"s);

    if (details::inTypelist(type, details::fileStrs) || details::inTypelist(type, details::rotateStrs) ||
        details::inTypelist(type, details::dailyStrs)) {
        handleTruncatableSink(sink, sinkPtr);
    }

    else if (details::inTypelist(type, details::nullStrs) || details::inTypelist(type, details::stdStrs)) {
        sinkPtr = details::genFromNullOrStdStr(sink);
    }

    else if (details::inTypelist(type, details::linuxStrs)) {
#ifdef __linux__
        sinkPtr = details::genFromLinuxStr(sink);
#else
        return;
#endif
    } else if (details::inTypelist(type, details::winStrs)) {
#ifdef _WIN32
        sinkPtr = details::genFromWinStr(sink);
#else
        return;
#endif
    }

    if (sink.level.has_value() && !sink.level->empty()) {
        if (!details::levelMap.contains(*sink.level))
            throw std::out_of_range("KDSPDSetup: level "s + *sink.level + " not found"s);

        sinkPtr->set_level(details::levelMap.at(*sink.level));
    }

    details::SPDMaps::emplaceSinkMap(std::make_pair(name, sinkPtr));
}

void setupSinks(Configuration const &config)
{
    for (auto const &sink : config.sink) {
        setupSink(sink);
    }
}

void setupPatterns(Configuration const &config)
{
    if (config.global_pattern.has_value()) {
        spdlog::set_pattern(*config.global_pattern);
    }

    for (auto const &pattern : config.pattern) {
        details::SPDMaps::emplacePatternMap(std::make_pair(pattern.name, pattern.value));
    }
}

void setupThreadPools(Configuration const &config)
{
    if (config.global_thread_pool.has_value()) {
        spdlog::init_thread_pool(config.global_thread_pool->queue_size, config.global_thread_pool->num_threads);
    }

    for (auto const &threadPool : config.thread_pool) {
        details::SPDMaps::emplaceThreadPoolMap(
                std::make_pair(threadPool.name, std::make_pair(threadPool.queue_size, threadPool.num_threads)));
    }
}

void registerAsynchronousLogger(LoggerConfig const &logger, std::vector<spdlog::sink_ptr> const &sinkList,
                                std::string const &pattern)
{
    static std::shared_ptr<spdlog::details::thread_pool> threadPoolPtr;

    if (logger.thread_pool.has_value() && !logger.thread_pool->empty()) {
        auto const &threadPool = *logger.thread_pool;

        if (!details::SPDMaps::threadPoolMap().contains(threadPool))
            throw std::out_of_range("KDSPDSetup: threadpool "s + threadPool + " not found"s);

        auto const threadPoolPair = details::SPDMaps::threadPoolMap().at(threadPool);
        auto const queueSize = threadPoolPair.first;
        auto const numThreads = threadPoolPair.second;
        threadPoolPtr = std::make_shared<spdlog::details::thread_pool>(queueSize, numThreads);
    } else {
        threadPoolPtr = spdlog::thread_pool();
    }

    auto const overflowPolicy = logger.overflow_policy.value_or("block");

    if (!details::overflowMap.contains(overflowPolicy))
        throw std::out_of_range("KDSPDSetup: overflow policy "s + overflowPolicy + " not found"s);

    auto asyncLogger = std::make_shared<spdlog::async_logger>(logger.name, sinkList.begin(), sinkList.end(),
                                                              std::move(threadPoolPtr),
                                                              details::overflowMap.at(overflowPolicy));

    if (!pattern.empty()) {
        asyncLogger->set_pattern(pattern);
    }

    spdlog::register_logger(asyncLogger);
}

void registerSynchronousLogger(LoggerConfig const &logger, std::vector<spdlog::sink_ptr> const &sinkList,
                               std::string const &pattern)
{
    auto syncLogger = std::make_shared<spdlog::logger>(logger.name, sinkList.cbegin(), sinkList.cend());
    if (!pattern.empty()) {
        syncLogger->set_pattern(pattern);
    }

    if (logger.level.has_value() && !logger.level->empty()) {
        if (!details::levelMap.contains(*logger.level))
            throw std::out_of_range("KDSPDSetup: level "s + *logger.level + " not found"s);

        syncLogger->set_level(details::levelMap.at(*logger.level));
    }

    spdlog::register_logger(syncLogger);
}

void setupLogger(LoggerConfig const &logger)
{
    auto const &name = logger.name;
    if (spdlog::get(name) != nullptr) {
        return;
    }

    auto sinkList = std::vector<spdlog::sink_ptr>{};
    for (auto const &sinkName : logger.sinks) {
        if (details::SPDMaps::sinkMap().contains(sinkName)) {
            sinkList.emplace_back(details::SPDMaps::sinkMap().at(sinkName));
        } else {
            // ignore sink instead of throwing, but notify that this happens
            std::cerr << "KDSPDSetup: setting up logger " << name << " - skipped sink " << sinkName
                      << " as it was not found";
        }
    }

    auto pattern = std::string{};

    if (logger.pattern.has_value() && !logger.pattern->empty()) {
        if (!details::SPDMaps::patternMap().contains(*logger.pattern))
            throw std::out_of_range("KDSPDSetup: pattern "s + *logger.pattern + " not found"s);

        pattern = details::SPDMaps::patternMap().at(*logger.pattern);
    }

    if (logger.type == "async") {
        registerAsynchronousLogger(logger, sinkList, pattern);
    } else {
        registerSynchronousLogger(logger, sinkList, pattern);
    }
}

void setupLoggers(Configuration const &config)
{
    for (auto const &logger : config.logger) {
        setupLogger(logger);
    }
}

} // namespace KDSPDSetup::setup
