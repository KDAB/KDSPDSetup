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

template<typename Mutex>
auto createRotatingFileSinkPtr(toml::table const &sinkTable, toml::string &&baseFilename,
                               std::size_t const &maxFiles)
        -> std::shared_ptr<spdlog::sinks::rotating_file_sink<Mutex>>
{
    auto tup = createRotatingFileSinkTuple(sinkTable, std::move(baseFilename), maxFiles);
    return std::make_shared<spdlog::sinks::rotating_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup),
                                                                      std::get<2>(tup));
}

auto createFileSinkTuple(toml::table const &sinkTable, bool const &trunct) -> std::tuple<toml::string const, bool const>
{
    auto fileName = sinkTable.at("filename").as_string();
    return std::make_tuple(std::move(fileName), trunct);
}

template<typename Mutex>
auto createFileSinkPtr(toml::table const &sinkTable, bool const &trunct)
        -> std::shared_ptr<spdlog::sinks::basic_file_sink<Mutex>>
{
    auto tup = createFileSinkTuple(sinkTable, trunct);
    return std::make_shared<spdlog::sinks::basic_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup));
}

auto createDailyFileSinkTuple(toml::table &&sinkTable, bool const &trunct, toml::string &&baseFilename,
                              uint16_t const &maxFiles)
        -> std::tuple<toml::string const, int const, int const, bool const, uint16_t const>
{
    auto rotationHour = static_cast<int>(sinkTable.at("rotation_hour").as_integer());
    auto rotationMinute = static_cast<int>(sinkTable.at("rotation_minute").as_integer());

    return std::make_tuple(std::move(baseFilename), rotationHour, rotationMinute, trunct, maxFiles);
}

template<typename Mutex>
auto createDailyFileSinkPtr(toml::table &&sinkTable, bool const &trunct, toml::string &&baseFilename,
                            uint16_t const &maxFiles) -> std::shared_ptr<spdlog::sinks::daily_file_sink<Mutex>>
{
    auto tup = createDailyFileSinkTuple(std::move(sinkTable), trunct, std::move(baseFilename), maxFiles);
    return std::make_shared<spdlog::sinks::daily_file_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup),
                                                                   std::get<3>(tup), std::get<4>(tup));
}

auto createNullSinkPtr() -> std::shared_ptr<spdlog::sinks::null_sink<spdlog::details::null_mutex>>
{
    return std::make_shared<spdlog::sinks::null_sink<spdlog::details::null_mutex>>();
}

template<typename Mutex>
auto createStdoutSinkPtr() -> std::shared_ptr<spdlog::sinks::stdout_sink<Mutex>>
{
    return std::make_shared<spdlog::sinks::stdout_sink<Mutex>>();
}

//////// needs work
// template<typename Mutex>
// auto createStdoutColorSinkPtr() -> std::shared_ptr<spdlog::sinks::stdout_color_sink_st<Mutex>>
// {
//     return std::make_shared<spdlog::sinks::stdout_color_sink<Mutex>>();
// }

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

template<typename Mutex>
auto createSyslogSinkPtr(toml::table const &sinkTable) -> std::shared_ptr<spdlog::sinks::syslog_sink<Mutex>>
{
    auto tup = createSyslogSinkTuple(sinkTable);
    return std::make_shared<spdlog::sinks::syslog_sink<Mutex>>(std::get<0>(tup), std::get<1>(tup), std::get<2>(tup),
                                                               std::get<3>(tup));
}

#elif _WIN32
template<typename Mutex>
auto createMsvcSinkPtr() -> std::shared_ptr<spdlog::sinks::msvc_sink<Mutex>>
{
    return std::make_shared<spdlog::sinks::msvc_sink<Mutex>>();
}
#endif

auto genFromFileStr(toml::string &&typeStr, toml::table &&sinkTable, bool const &trunct) -> spdlog::sink_ptr
{
    if (typeStr == "basic_file_sink_st") {
        return createFileSinkStPtr(sinkTable, trunct);
    }
    if (typeStr == "basic_file_sink_mt") {
        return createFileSinkMtPtr(sinkTable, trunct);
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

auto genFromDailyStr(toml::string &&typeStr, toml::table &&sinkTable, bool const &trunct,
                     toml::string &&baseFilename, uint16_t const &maxFiles) -> spdlog::sink_ptr
{
    if (typeStr == "daily_file_sink_st") {
        return createDailyFileSinkStPtr(std::move(sinkTable), trunct, std::move(baseFilename), maxFiles);
    }
    if (typeStr == "daily_file_sink_mt") {
        return createDailyFileSinkMtPtr(std::move(sinkTable), trunct, std::move(baseFilename), maxFiles);
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
        return std::make_shared<spdlog::sinks::stdout_color_sink_st>(); // still needs work
    }
    if (typeStr == "stdout_color_sink_mt" || typeStr == "color_stdout_sink_mt") {
        return std::make_shared<spdlog::sinks::stdout_color_sink_mt>(); // still needs work
    }

    return nullptr;
}

#ifdef __linux__
auto genFromLinuxStr(toml::string &&typeStr, toml::table &&sinkTable) -> spdlog::sink_ptr
{
    if (typeStr == "syslog_sink_st") {
        return createSyslogSinkStPtr(sinkTable);
    }
    if (typeStr == "syslog_sink_mt") {
        return createSyslogSinkMtPtr(sinkTable);
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
