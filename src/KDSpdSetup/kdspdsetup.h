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

#include <string>

/**
 * @brief The only public-facing namespace in the library.
 * 
 */
namespace KDSPDSetup {

/**
 * @brief This function performs all setup needed to start logging.
 * It should be the only callable part of the library.
 * 
 * @param file The toml configuration file to read from.
 */
void setupFrom(std::string const &file);

} // namespace KDSPDSetup
