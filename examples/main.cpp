#include "../include/kdspdsetup/kdspdsetup.h"
#include <iostream>

int main() {
    KDSPDSetup::KDSPDSetup::setup_from (
        "../examples/example.toml" // from build directory
    );
    
    auto logger_root = spdlog::get("root");
    logger_root->debug("hello root!");
    logger_root->critical("hello root!");
    logger_root->error("hello root!");
    logger_root->info("hello root!");
    logger_root->trace("hello root!");
    logger_root->warn("hello root!");

    auto logger_console = spdlog::get("console");
    logger_console->debug("hello console!");
    logger_console->critical("hello console!");
    logger_console->error("hello console!");
    logger_console->info("hello console!");
    logger_console->trace("hello console!");
    logger_console->warn("hello console!");

    auto logger_global_async = spdlog::get("global_async");
    logger_global_async->debug("hello global_async!");
    logger_global_async->critical("hello global_async!");
    logger_global_async->error("hello global_async!");
    logger_global_async->info("hello global_async!");
    logger_global_async->trace("hello global_async!");
    logger_global_async->warn("hello global_async!");

    auto logger_local_async = spdlog::get("local_async");
    logger_local_async->debug("hello local_async!");
    logger_local_async->critical("hello local_async!");
    logger_local_async->error("hello local_async!");
    logger_local_async->info("hello local_async!");
    logger_local_async->trace("hello local_async!");
    logger_local_async->warn("hello local_async!");
}
