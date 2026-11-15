/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "details.h"

#include <algorithm>
#include <stdexcept>

using std::literals::string_literals::operator""s;

namespace KDSPDSetup::details {

namespace {

/// @brief Return a field that a sink type requires, or throw if the configuration did not provide it.
auto required(std::optional<std::string> const &field, char const *fieldName) -> std::string
{
    if (!field.has_value() || field->empty()) {
        throw std::runtime_error("KDSPDSetup: missing required field "s + fieldName + ".");
    }
    return *field;
}

} // namespace

auto SPDMaps::sinkMap() -> std::map<std::string, spdlog::sink_ptr> const &
{
    return mSinkMap;
}
auto SPDMaps::patternMap() -> std::map<std::string, std::string> const &
{
    return mPatternMap;
}
auto SPDMaps::threadPoolMap() -> std::map<std::string, std::pair<std::size_t, std::size_t>> const &
{
    return mThreadPoolMap;
}
void SPDMaps::emplaceSinkMap(std::pair<std::string, spdlog::sink_ptr> &&_pr)
{
    mSinkMap.emplace(_pr);
}
void SPDMaps::emplacePatternMap(std::pair<std::string, std::string> &&_pr)
{
    mPatternMap.emplace(_pr);
}
void SPDMaps::emplaceThreadPoolMap(std::pair<std::string, std::pair<std::size_t, std::size_t>> &&_pr)
{
    mThreadPoolMap.emplace(_pr);
}

bool inTypelist(std::string const &typeStr, std::vector<std::string> const &strList)
{
    return std::find(strList.cbegin(), strList.cend(), typeStr) != std::cend(strList);
}

auto createRotatingFileSinkTuple(SinkConfig const &sink) -> std::tuple<std::string, std::size_t, std::size_t>
{
    auto const maxSize = required(sink.max_size, "max_size");
    auto const maxSizeStrBack = maxSize.back();
    std::size_t maxSizeInt{};

    if (std::string{ "TGMK" }.find(maxSizeStrBack) != std::string::npos) {
        auto const prefixStr = maxSize.substr(0, maxSize.size() - 1);
        maxSizeInt = std::stoi(prefixStr);

        std::size_t constexpr kilo = 1024;
        std::size_t constexpr mega = kilo * kilo;
        std::size_t constexpr giga = mega * kilo;
        std::size_t constexpr tera = giga * kilo;

        switch (maxSizeStrBack) {
        case 'T':
            maxSizeInt *= tera;
            break;
        case 'G':
            maxSizeInt *= giga;
            break;
        case 'M':
            maxSizeInt *= mega;
            break;
        case 'K':
            maxSizeInt *= kilo;
            break;
        default:
            break;
        }
    } else {
        maxSizeInt = std::stoi(maxSize);
    }

    return std::make_tuple(required(sink.base_filename, "base_filename"), maxSizeInt, sink.max_files);
}

auto createFileSinkTuple(SinkConfig const &sink) -> std::tuple<std::string, bool>
{
    return std::make_tuple(required(sink.filename, "filename"), sink.truncate);
}

auto createDailyFileSinkTuple(SinkConfig const &sink) -> std::tuple<std::string, int, int, bool, std::size_t>
{
    return std::make_tuple(required(sink.base_filename, "base_filename"), sink.rotation_hour, sink.rotation_minute,
                           sink.truncate, sink.max_files);
}

auto createNullSinkPtr() -> std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>>
{
    return std::make_shared<spdlog::sinks::null_sink<spdlog::details::null_mutex>>();
}

#ifdef __linux__
auto createSyslogSinkTuple(SinkConfig const &sink) -> std::tuple<std::string, int, int, bool>
{
    auto ident = sink.ident.value_or("");

    auto syslogOption = sink.syslog_option.value_or(int{ 0 });

    auto syslogFacility = sink.syslog_facility.value_or(LOG_USER); // macro

    bool enableFormatting = true;

    return std::make_tuple(std::move(ident), syslogOption, syslogFacility, enableFormatting);
}
#endif

auto genFromFileStr(SinkConfig const &sink) -> spdlog::sink_ptr
{
    if (sink.type == "basic_file_sink_st") {
        return createFileSinkStPtr(sink);
    }
    if (sink.type == "basic_file_sink_mt") {
        return createFileSinkMtPtr(sink);
    }

    return nullptr;
}

auto genFromRotateStr(SinkConfig const &sink) -> spdlog::sink_ptr
{
    if (sink.type == "rotating_file_sink_st") {
        return createRotatingFileSinkStPtr(sink);
    }
    if (sink.type == "rotating_file_sink_mt") {
        return createRotatingFileSinkMtPtr(sink);
    }

    return nullptr;
}

auto genFromDailyStr(SinkConfig const &sink) -> spdlog::sink_ptr
{
    if (sink.type == "daily_file_sink_st") {
        return createDailyFileSinkStPtr(sink);
    }
    if (sink.type == "daily_file_sink_mt") {
        return createDailyFileSinkMtPtr(sink);
    }

    return nullptr;
}

auto genFromNullOrStdStr(SinkConfig const &sink) -> spdlog::sink_ptr
{
    auto const &typeStr = sink.type;

    if (typeStr == "null_sink_st" || typeStr == "null_sink_mt") {
        return createNullSinkPtr();
    }
    if (typeStr == "stdout_sink_st") {
        return createStdoutSinkStPtr();
    }
    if (typeStr == "stdout_sink_mt") {
        return createStdoutSinkMtPtr();
    }
    if (typeStr == "stdout_color_sink_st" || typeStr == "color_stdout_sink_st") {
        return createStdoutColorSinkStPtr();
    }
    if (typeStr == "stdout_color_sink_mt" || typeStr == "color_stdout_sink_mt") {
        return createStdoutColorSinkMtPtr();
    }
    if (typeStr == "stderr_sink_st") {
        return createStderrSinkStPtr();
    }
    if (typeStr == "stderr_sink_mt") {
        return createStderrSinkMtPtr();
    }
    if (typeStr == "stderr_color_sink_st" || typeStr == "color_stderr_sink_st") {
        return createStderrColorSinkStPtr();
    }
    if (typeStr == "stderr_color_sink_mt" || typeStr == "color_stderr_sink_mt") {
        return createStderrColorSinkMtPtr();
    }

    return nullptr;
}

#ifdef __linux__
auto genFromLinuxStr(SinkConfig const &sink) -> spdlog::sink_ptr
{
    if (sink.type == "syslog_sink_st") {
        return createSyslogSinkStPtr(sink);
    }
    if (sink.type == "syslog_sink_mt") {
        return createSyslogSinkMtPtr(sink);
    }

    return nullptr;
}
#elif _WIN32
auto genFromWinStr(SinkConfig const &sink) -> spdlog::sink_ptr
{
    auto const &typeStr = sink.type;

    if (typeStr == "msvc_sink_st") {
        return createMsvcSinkStPtr();
    }
    if (typeStr == "msvc_sink_mt") {
        return createMsvcSinkMtPtr();
    }

    return nullptr;
}
#endif

} // namespace KDSPDSetup::details
