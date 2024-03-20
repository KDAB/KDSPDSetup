/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "details.h"

namespace KDSPDSetup::details {

auto SPDMaps::sinkMap() -> std::map<toml::string, spdlog::sink_ptr> const &
{
    return mSinkMap;
}
auto SPDMaps::patternMap() -> std::map<toml::string, toml::string> const &
{
    return mPatternMap;
}
auto SPDMaps::threadPoolMap() -> std::map<toml::string, std::pair<std::size_t, std::size_t>> const &
{
    return mThreadPoolMap;
}
void SPDMaps::emplaceSinkMap(std::pair<toml::string, spdlog::sink_ptr> &&_pr)
{
    mSinkMap.emplace(_pr);
}
void SPDMaps::emplacePatternMap(std::pair<toml::string, toml::string> &&_pr)
{
    mPatternMap.emplace(_pr);
}
void SPDMaps::emplaceThreadPoolMap(std::pair<toml::string, std::pair<std::size_t, std::size_t>> &&_pr)
{
    mThreadPoolMap.emplace(_pr);
}

bool inTypelist(std::string const &typeStr, std::vector<std::string> const &strList)
{
    return std::find(strList.cbegin(), strList.cend(), typeStr) != std::cend(strList);
}

auto createRotatingFileSinkTuple(toml::table const &sinkTable, toml::string &&baseFilename,
                                 std::size_t const &maxFiles)
        -> std::tuple<toml::string const, std::size_t const, std::size_t const>
{
    auto const maxSize = static_cast<std::string>(sinkTable.at("max_size").as_string());
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

    return std::make_tuple(std::move(baseFilename), maxSizeInt, maxFiles);
}

auto createFileSinkTuple(toml::table const &sinkTable, bool const &truncate) -> std::tuple<toml::string const, bool const>
{
    auto fileName = sinkTable.at("filename").as_string();
    return std::make_tuple(std::move(fileName), truncate);
}

auto createDailyFileSinkTuple(toml::table &&sinkTable, bool const &truncate, toml::string &&baseFilename,
                              uint16_t const &maxFiles)
        -> std::tuple<toml::string const, int const, int const, bool const, uint16_t const>
{
    auto rotationHour = static_cast<int>(sinkTable.at("rotation_hour").as_integer());
    auto rotationMinute = static_cast<int>(sinkTable.at("rotation_minute").as_integer());

    return std::make_tuple(std::move(baseFilename), rotationHour, rotationMinute, truncate, maxFiles);
}

auto createNullSinkPtr() -> std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>>
{
    return std::make_shared<spdlog::sinks::null_sink<spdlog::details::null_mutex>>();
}

#ifdef __linux__
auto createSyslogSinkTuple(toml::table const &sinkTable)
        -> std::tuple<toml::string const, int const, int const, bool const>
{
    auto ident = (sinkTable.contains("ident")) ? sinkTable.at("ident").as_string() : "";

    auto syslogOption = (sinkTable.contains("syslog_option")) ? static_cast<int>(sinkTable.at("syslog_option").as_integer()) : int{ 0 };

    auto syslogFacility =
            (sinkTable.contains("syslog_facility")) ? static_cast<int>(sinkTable.at("syslog_facility").as_integer()) : LOG_USER; // macro

    bool enableFormatting = true;

    return std::make_tuple(std::move(ident), syslogOption, syslogFacility, enableFormatting);
}
#endif

auto genFromFileStr(toml::string &&typeStr, toml::table &&sinkTable, bool const &truncate) -> spdlog::sink_ptr
{
    if (typeStr == "basic_file_sink_st") {
        return createFileSinkStPtr(sinkTable, truncate);
    }
    if (typeStr == "basic_file_sink_mt") {
        return createFileSinkMtPtr(sinkTable, truncate);
    }

    return nullptr;
}

auto genFromRotateStr(toml::string &&typeStr, toml::table &&sinkTable, toml::string &&baseFilename,
                      std::size_t const &maxFiles) -> spdlog::sink_ptr
{
    if (typeStr == "rotating_file_sink_st") {
        return createRotatingFileSinkStPtr(std::move(sinkTable), std::move(baseFilename), maxFiles);
    }
    if (typeStr == "rotating_file_sink_mt") {
        return createRotatingFileSinkMtPtr(std::move(sinkTable), std::move(baseFilename), maxFiles);
    }

    return nullptr;
}

auto genFromDailyStr(toml::string &&typeStr, toml::table &&sinkTable, bool const &truncate,
                     toml::string &&baseFilename, uint16_t const &maxFiles) -> spdlog::sink_ptr
{
    if (typeStr == "daily_file_sink_st") {
        return createDailyFileSinkStPtr(std::move(sinkTable), truncate, std::move(baseFilename), maxFiles);
    }
    if (typeStr == "daily_file_sink_mt") {
        return createDailyFileSinkMtPtr(std::move(sinkTable), truncate, std::move(baseFilename), maxFiles);
    }

    return nullptr;
}

auto genFromNullOrStdStr(toml::string &&typeStr) -> spdlog::sink_ptr
{
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
auto genFromLinuxStr(toml::string &&typeStr, toml::table &&sinkTable) -> spdlog::sink_ptr
{
    if (typeStr == "syslog_sink_st") {
        return createSyslogSinkStPtr(std::move(sinkTable));
    }
    if (typeStr == "syslog_sink_mt") {
        return createSyslogSinkMtPtr(std::move(sinkTable));
    }

    return nullptr;
}
#elif _WIN32
auto genFromWinStr(toml::string &&typeStr) -> spdlog::sink_ptr
{
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
