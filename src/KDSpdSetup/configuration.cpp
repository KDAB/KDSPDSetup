/*
  This file is part of KDSpdSetup.

  SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

  SPDX-License-Identifier: MIT

  Contact KDAB at <info@kdab.com> for commercial licensing options.
*/
#include "configuration.h"

#include "details.h"

#include <algorithm>
#include <cctype>
#include <fstream>
#include <iterator>
#include <stdexcept>

#include <rfl/json.hpp>
#include <rfl/toml.hpp>
#include <rfl/yaml.hpp>

using std::literals::string_literals::operator""s;

namespace KDSPDSetup::configuration {

namespace {

/// @brief Lower-case a string so extensions compare case-insensitively.
auto toLower(std::string_view str) -> std::string
{
    auto lowered = std::string{ str };
    std::transform(lowered.begin(), lowered.end(), lowered.begin(), [](unsigned char chr) {
        return static_cast<char>(std::tolower(chr));
    });
    return lowered;
}

/// @brief Run one reflect-cpp reader and turn its result into a configuration or an exception.
template<typename Reader>
auto parse(std::string_view content, Reader &&reader, char const *formatName) -> Configuration
{
    auto result = reader(content);

    if (!result) {
        throw std::runtime_error("KDSPDSetup: could not parse the "s + formatName + " configuration: "
                                 + result.error().what());
    }

    return result.value();
}

/// @brief Throw unless `sink` carries a non-empty `field`.
void requireField(SinkConfig const &sink, std::optional<std::string> const &field, char const *fieldName)
{
    if (!field.has_value() || field->empty()) {
        throw std::runtime_error("KDSPDSetup: sink "s + sink.name + " of type " + sink.type
                                 + " requires the field " + fieldName + ".");
    }
}

/// @brief Check the fields a sink needs on top of `name` and `type`.
void validateSink(SinkConfig const &sink)
{
    if (sink.name.empty()) {
        throw std::runtime_error("KDSPDSetup: every sink requires a name."s);
    }
    if (sink.type.empty()) {
        throw std::runtime_error("KDSPDSetup: sink "s + sink.name + " requires a type.");
    }

    if (details::inTypelist(sink.type, details::fileStrs)) {
        requireField(sink, sink.filename, "filename");
    } else if (details::inTypelist(sink.type, details::rotateStrs)) {
        requireField(sink, sink.base_filename, "base_filename");
        requireField(sink, sink.max_size, "max_size");
    } else if (details::inTypelist(sink.type, details::dailyStrs)) {
        requireField(sink, sink.base_filename, "base_filename");
    }
}

/// @brief Read the whole content of `path`, or throw if it cannot be opened.
auto readFile(std::string const &path) -> std::string
{
    auto stream = std::ifstream{ path, std::ios::binary };
    if (!stream) {
        throw std::runtime_error("KDSPDSetup: could not open "s + path + ".");
    }

    return std::string{ std::istreambuf_iterator<char>{ stream }, std::istreambuf_iterator<char>{} };
}

} // namespace

auto formatFromExtension(std::string_view path) -> std::optional<Format>
{
    auto const dot = path.find_last_of('.');
    if (dot == std::string_view::npos) {
        return std::nullopt;
    }

    auto const extension = toLower(path.substr(dot + 1));

    if (extension == "toml") {
        return Format::toml;
    }
    if (extension == "json") {
        return Format::json;
    }
    if (extension == "yaml" || extension == "yml") {
        return Format::yaml;
    }

    return std::nullopt;
}

void validate(Configuration const &config)
{
    for (auto const &sink : config.sink) {
        validateSink(sink);
    }

    for (auto const &pattern : config.pattern) {
        if (pattern.name.empty()) {
            throw std::runtime_error("KDSPDSetup: every pattern requires a name."s);
        }
        if (pattern.value.empty()) {
            throw std::runtime_error("KDSPDSetup: pattern "s + pattern.name + " requires a value.");
        }
    }

    for (auto const &threadPool : config.thread_pool) {
        if (threadPool.name.empty()) {
            throw std::runtime_error("KDSPDSetup: every thread pool requires a name."s);
        }
    }

    for (auto const &logger : config.logger) {
        if (logger.name.empty()) {
            throw std::runtime_error("KDSPDSetup: every logger requires a name."s);
        }
    }
}

auto loadFromString(std::string_view content, Format format) -> Configuration
{
    Configuration config;

    switch (format) {
    case Format::toml:
        config = parse(
                content,
                [](std::string_view str) { return rfl::toml::read<Configuration, rfl::DefaultIfMissing>(str); },
                "TOML");
        break;
    case Format::json:
        config = parse(
                content,
                [](std::string_view str) { return rfl::json::read<Configuration, rfl::DefaultIfMissing>(str); },
                "JSON");
        break;
    case Format::yaml:
        config = parse(
                content,
                [](std::string_view str) { return rfl::yaml::read<Configuration, rfl::DefaultIfMissing>(str); },
                "YAML");
        break;
    }

    validate(config);

    return config;
}

auto load(std::string const &path) -> Configuration
{
    auto const format = formatFromExtension(path);

    if (!format.has_value()) {
        throw std::invalid_argument("KDSPDSetup: cannot tell the format of "s + path
                                    + " from its extension. Use .toml, .json, .yaml or .yml.");
    }

    return loadFromString(readFile(path), *format);
}

auto load(std::string const &path, Format format) -> Configuration
{
    return loadFromString(readFile(path), format);
}

} // namespace KDSPDSetup::configuration
