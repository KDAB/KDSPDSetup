#include "../include/kdspdsetup/kdspdsetup.h"
#include <iostream>

int main()
{
    KDSPDSetup::KDSPDSetup::setup_from("../examples/example.toml"); // from build directory

    for (std::string &&name : {"root", "console", "global_async", "local_async"})
    {
        auto logger = spdlog::get(name);
        auto message = fmt::format("hello {}", name);
        logger->debug(message);
        logger->critical(message);
        logger->error(message);
        logger->info(message);
        logger->trace(message);
        logger->warn(message);
    }
}
