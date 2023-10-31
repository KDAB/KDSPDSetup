#include "../include/kdspdsetup.h"
#include "setup.h"

namespace KDSPDSetup {

void setup_from(std::string const &file)
{
    auto const data = toml::parse(file);

    setup::setup_sinks(data);
    setup::setup_patterns(data);
    setup::setup_threadpools(data);
    setup::setup_loggers(data);
}

} // namespace KDSPDSetup
