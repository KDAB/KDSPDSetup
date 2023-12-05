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

#include <string>

namespace KDSPDSetup {

void setupFrom(std::string const &file);

} // namespace KDSPDSetup
