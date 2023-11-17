#include <../include/kdspdsetup.h>
#include "setup.h"

namespace KDSPDSetup {

void setupFrom(std::string const &file)
{
    auto const data = toml::parse(file);

    setup::setupSinks(data);
    setup::setupPatterns(data);
    setup::setupThreadPools(data);
    setup::setupLoggers(data);
}

} // namespace KDSPDSetup
