#pragma once

#include <kdspdsetup.h>

#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN

void setup(std::string &&str)
{
    KDSPDSetup::setupFrom(str);
}
