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

void handleMultipleFileSink(toml::string &&typeStr, toml::table &&sinkTable, spdlog::sink_ptr &sinkPtr, bool const &truncate)
{
    auto baseFilename = sinkTable.at("base_filename").as_string();
    auto maxFiles = (sinkTable.contains("max_files")) ? static_cast<uint16_t>(sinkTable.at("max_files").as_integer()) : uint16_t{ 0 };

    if (details::inTypelist(typeStr, details::rotateStrs)) {
        sinkPtr = details::genFromRotateStr(std::move(typeStr), std::move(sinkTable), std::move(baseFilename), maxFiles);
    }

    else if (details::inTypelist(typeStr, details::dailyStrs)) {
        sinkPtr = details::genFromDailyStr(std::move(typeStr), std::move(sinkTable), truncate, std::move(baseFilename), maxFiles);
    }
}

void handleTruncatableSink(toml::string &&typeStr, toml::table &&sinkTable, spdlog::sink_ptr &sinkPtr)
{
    auto const truncate = (sinkTable.contains("truncate")) ? sinkTable.at("truncate").as_boolean() : false;

    if (details::inTypelist(typeStr, details::fileStrs)) {
        sinkPtr = details::genFromFileStr(std::move(typeStr), std::move(sinkTable), truncate);
    } else if (details::inTypelist(typeStr, details::rotateStrs) || details::inTypelist(typeStr, details::dailyStrs)) {
        handleMultipleFileSink(std::move(typeStr), std::move(sinkTable), sinkPtr, truncate);
    }
}

void setupSink(toml::table &&sinkTable)
{
    spdlog::sink_ptr sinkPtr;

    auto const name = sinkTable.at("name").as_string();
    auto typeStr = sinkTable.at("type").as_string();

    bool ok = false;
    for (auto &typeList : { details::fileStrs, details::rotateStrs, details::dailyStrs, details::nullStrs, details::stdStrs, details::linuxStrs, details::winStrs, details::androidStrs }) {
        if (details::inTypelist(typeStr, typeList)) {
            ok = true;
            break;
        }
    }

    if (!ok)
        throw std::out_of_range("KDSPDSetup: sink type "s + typeStr.str + " is not valid"s);

    toml::string level = "";
    if (sinkTable.contains("level")) {
        level = sinkTable.at("level").as_string();
    }

    if (details::inTypelist(typeStr, details::fileStrs) || details::inTypelist(typeStr, details::rotateStrs) ||
        details::inTypelist(typeStr, details::dailyStrs)) {
        handleTruncatableSink(std::move(typeStr), std::move(sinkTable), sinkPtr);
    }

    else if (details::inTypelist(typeStr, details::nullStrs) || details::inTypelist(typeStr, details::stdStrs)) {
        sinkPtr = details::genFromNullOrStdStr(std::move(typeStr));
    }

    else if (details::inTypelist(typeStr, details::linuxStrs)) {
#ifdef __linux__
        sinkPtr = details::genFromLinuxStr(std::move(typeStr), std::move(sinkTable));
#else
        return;
#endif
    } else if (details::inTypelist(typeStr, details::winStrs)) {
#ifdef _WIN32
        sinkPtr = details::genFromWinStr(std::move(typeStr));
#else
        return;
#endif
    } else if (details::inTypelist(typeStr, details::androidStrs)) {
#ifdef __ANDROID__
        auto tag = (sinkTable.contains("tag")) ? sinkTable.at("tag").as_string() : "spdlog";
        auto const useRawMsg = (sinkTable.contains("use_raw_msg")) ? sinkTable.at("use_raw_msg").as_boolean() : false;
        sinkPtr = details::genFromAndroidStr(std::move(typeStr), std::move(tag), useRawMsg);
#else
        return;
#endif
    }

    if (level != "") {
        if (!details::levelMap.contains(level))
            throw std::out_of_range("KDSPDSetup: level "s + level.str + " not found"s);

        sinkPtr->set_level(details::levelMap.at(level));
    }

    details::SPDMaps::emplaceSinkMap(std::make_pair(name, sinkPtr));
}

void setupSinks(toml::value const &data)
{
    auto sinks = toml::find<std::vector<toml::table>>(data, "sink");

    for (auto &&sinkTable : sinks) {
        setupSink(std::move(sinkTable));
    }
}

void setupPatterns(toml::value const &data)
{
    if (data.contains("global_pattern")) {
        auto const globalPattern = toml::find<toml::string>(data, "global_pattern");
        spdlog::set_pattern(globalPattern);
    }

    if (data.contains("pattern")) {
        auto const patterns = toml::find<std::vector<toml::table>>(data, "pattern");

        for (auto &&patternTable : patterns) {
            auto const name = patternTable.at("name").as_string();
            auto const value = patternTable.at("value").as_string();

            details::SPDMaps::emplacePatternMap(std::make_pair(name, value));
        }
    }
}

void setupThreadPools(toml::value const &data)
{
    if (data.contains("global_thread_pool")) {
        auto const globalThreadPool = toml::find(data, "global_thread_pool");

        auto const queueSize = toml::find<std::size_t>(globalThreadPool, "queue_size");
        auto const numThreads = toml::find<std::size_t>(globalThreadPool, "num_threads");
        spdlog::init_thread_pool(queueSize, numThreads);
    }

    if (data.contains("thread_pool")) {
        auto const threadPools = toml::find<std::vector<toml::table>>(data, "thread_pool");

        for (auto &&poolTable : threadPools) {
            auto const name = poolTable.at("name").as_string();
            auto const queueSize = static_cast<std::size_t>(poolTable.at("queue_size").as_integer());
            auto const numThreads = static_cast<std::size_t>(poolTable.at("num_threads").as_integer());

            details::SPDMaps::emplaceThreadPoolMap(std::make_pair(name, std::make_pair(queueSize, numThreads)));
        }
    }
}

void registerAsynchronousLogger(toml::table const &loggerTable, toml::string const &name, std::vector<spdlog::sink_ptr> const &sinkList,
                                toml::string const &pattern)
{
    auto const threadPool = (loggerTable.contains("thread_pool")) ? loggerTable.at("thread_pool").as_string() : "";

    static std::shared_ptr<spdlog::details::thread_pool> threadPoolPtr;

    if (threadPool != "") {
        if (!details::SPDMaps::threadPoolMap().contains(threadPool))
            throw std::out_of_range("KDSPDSetup: threadpool "s + threadPool.str + " not found"s);

        auto const threadPoolPair = details::SPDMaps::threadPoolMap().at(threadPool);
        auto const queueSize = threadPoolPair.first;
        auto const numThreads = threadPoolPair.second;
        threadPoolPtr = std::make_shared<spdlog::details::thread_pool>(queueSize, numThreads);
    } else {
        threadPoolPtr = spdlog::thread_pool();
    }

    auto const overflowPolicy =
            (loggerTable.contains("overflow_policy")) ? loggerTable.at("overflow_policy").as_string() : "block";

    if (!details::overflowMap.contains(overflowPolicy))
        throw std::out_of_range("KDSPDSetup: overflow policy "s + overflowPolicy.str + " not found"s);

    auto logger = std::make_shared<spdlog::async_logger>(name, sinkList.begin(), sinkList.end(), std::move(threadPoolPtr),
                                                         details::overflowMap.at(overflowPolicy));

    if (pattern != "") {
        logger->set_pattern(pattern);
    }

    spdlog::register_logger(logger);
}

void registerSynchronousLogger(toml::table const &loggerTable, toml::string const &name, std::vector<spdlog::sink_ptr> const &sinkList,
                               toml::string const &pattern)
{
    auto logger = std::make_shared<spdlog::logger>(name, sinkList.cbegin(), sinkList.cend());
    if (pattern != "") {
        logger->set_pattern(static_cast<std::string>(pattern));
    }

    if (loggerTable.contains("level")) {
        auto const level = loggerTable.at("level").as_string();

        if (!details::levelMap.contains(level))
            throw std::out_of_range("KDSPDSetup: level "s + level.str + " not found"s);

        logger->set_level(details::levelMap.at(level));
    }

    spdlog::register_logger(logger);
}

void setupLogger(toml::table const &loggerTable)
{
    auto const name = loggerTable.at("name").as_string();
    if (spdlog::get(name) != nullptr) {
        return;
    }

    auto const sinks = loggerTable.at("sinks").as_array();

    auto sinkList = std::vector<spdlog::sink_ptr>{};
    for (auto &&sink : sinks) {
        if (details::SPDMaps::sinkMap().contains(sink.as_string())) {
            sinkList.emplace_back(details::SPDMaps::sinkMap().at(sink.as_string()));
        } else {
            // ignore sink instead of throwing, but notify that this happens
            std::cerr << "KDSPDSetup: setting up logger " << name
                      << " - skipped sink " << sink.as_string().str << " as it was not found";
        }
    }

    std::string pattern = "";

    if (loggerTable.contains("pattern")) {
        if (!details::SPDMaps::patternMap().contains(loggerTable.at("pattern").as_string()))
            throw std::out_of_range("KDSPDSetup: pattern "s + loggerTable.at("pattern").as_string().str + " not found"s);

        pattern = details::SPDMaps::patternMap().at(loggerTable.at("pattern").as_string());
    }

    auto const type = (loggerTable.contains("type")) ? loggerTable.at("type").as_string() : "";

    if (type == "async") {
        registerAsynchronousLogger(loggerTable, name, sinkList, pattern);
    } else {
        registerSynchronousLogger(loggerTable, name, sinkList, pattern);
    }
}

void setupLoggers(toml::value const &data)
{
    auto const loggers = toml::find<std::vector<toml::table>>(data, "logger");
    for (auto &&loggerTable : loggers) {
        setupLogger(loggerTable);
    }
}

} // namespace KDSPDSetup::setup
