#pragma once

#include "details.h"

namespace KDSPDSetup::setup {

void handle_multifiles(toml::string &&typestr, toml::table &&sinktb, spdlog::sink_ptr &sinkp, bool const &trunct);

void handle_truncatable(toml::string &&typestr, toml::table &&sinktb, spdlog::sink_ptr &sinkp);

void setup_sink(toml::table &&sinktb);

void setup_sinks(toml::value const &data);

void setup_patterns(toml::value const &data);

void setup_threadpools(toml::value const &data);

void register_async(toml::table const &logtb, toml::string const &name, std::vector<spdlog::sink_ptr> const &sinklist,
                    toml::string const &pattern);

void register_syncr(toml::table const &logtb, toml::string const &name, std::vector<spdlog::sink_ptr> const &sinklist,
                    toml::string const &pattern);

void setup_logger(toml::table const &logtb);

void setup_loggers(toml::value const &data);

} // namespace KDSPDSetup::setup
