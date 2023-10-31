<div align=center>

```
            ▄────▄ •▄ ·▄▄▄▄  .▄▄ ·  ▄▄▄··▄▄▄▄  .▄▄ · ▄▄▄ .▄▄▄▄▄▄• ▄▌ ▄▄▄·
         ▄██─────█▌▄▌▪██▪ ██ ▐█ ▀. ▐█ ▄███▪ ██ ▐█ ▀. ▀▄.▀·•██  █▪██▌▐█ ▄█
   ▄▀██▀█▀█▀███▀─▐▀▀▄·▐█· ▐█▌▄▀▀▀█▄ ██▀·▐█· ▐█▌▄▀▀▀█▄▐▀▀▪▄ ▐█.▪█▌▐█▌ ██▀·
 ▀▀▀▀▀████▀▀▀────▐█.█▌██. ██ ▐█▄▪▐█▐█▪·•██. ██ ▐█▄▪▐█▐█▄▄▌ ▐█▌·▐█▄█▌▐█▪·•
       ▀██───────·▀  ▀▀▀▀▀▀•  ▀▀▀▀ .▀   ▀▀▀▀▀•  ▀▀▀▀  ▀▀▀  ▀▀▀  ▀▀▀ .▀   
```

![C++](https://img.shields.io/badge/c++-%2300599C.svg?style=for-the-badge&logo=c%2B%2B&logoColor=white)
![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)
![Git](https://img.shields.io/badge/git-%23F05033.svg?style=for-the-badge&logo=git&logoColor=white)
![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)
![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)

</div>

# KDSPDSetup

## Overview

KDSPDSetup is minimal reimplementation of the unmaintained [spdlog_setup](https://github.com/guangie88/spdlog_setup)
library for modern C++ projects.

Given a TOML file containing configuration parameters, `spdlog` loggers for your project can be set up with a single
call to `KDSPDSetup::setup_from`, as shown below:

```cpp
#include <kdspdsetup.h>

int main()
{
    KDSPDSetup::setup_from("example.toml");

    auto logger = spdlog::get("root");
    logger->debug("Hello spdlog!");
}

// [2023-10-30 14:35:19.244] [root] [debug] Hello spdlog!

```

## Requirements

You will need a compiler that supports the C++20 standard.

The following should be sufficient (still needs testing):

| ![GNU Badge](https://img.shields.io/badge/gcc-A42E2B?logo=gnu&logoColor=fff&style=for-the-badge) | ![LLVM Badge](https://img.shields.io/badge/clang-262D3A?logo=llvm&logoColor=fff&style=for-the-badge) | ![Visual Studio Badge](https://img.shields.io/badge/msvc-5C2D91?logo=visualstudio&logoColor=fff&style=for-the-badge) | ![NVIDIA Badge](https://img.shields.io/badge/nvcc-76B900?logo=nvidia&logoColor=fff&style=for-the-badge) | ![Intel Badge](https://img.shields.io/badge/Intel%20C++-0071C5?logo=intel&logoColor=fff&style=for-the-badge) |
| ------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- | --- | ---
| GCC 10+                                                                                          | Clang 8+                                                                                             | MSVC 19.22+                                                                                                          | NVCC 12.0+ | Intel C++ 2021.1+

## Dependencies
Aside from the obvious dependencies [spdlog](https://github.com/gabime/spdlog) and
[fmt](https://github.com/fmtlib/fmt), there are two additional dependencies:

| Purpose      | Lib                                           | Needed     |
| ------------ | --------------------------------------------- | ---------- |
| TOML parsing | [toml11](https://github.com/ToruNiina/toml11) | Always     |
| Testing      | [doctest](https://github.com/doctest/doctest) | Tests only |

These dependencies will be downloaded as submodules if not found. (WIP)

## Build & Install

### Build

You'll need at least CMake 3.5 to build this library.

To build as a static library, pass the flag `-DBUILD_STATIC` to CMake. Otherwise, it will build as a shared library by default.

### Install

Install <...> (WIP)

### Tests

You can build the unit tests by passing the flag `-DBUILD_TESTS` to CMake. The dependency [doctest](https://github.com/doctest/doctest)
will only be used when this flag is on.

### Examples

To build the basic examples, pass the flag `-DBUILD_EXAMPLES` to CMake.

## Configuration

How to write config TOML file

## Licensing

:)

## About KDAB

KDSPDSetup is supported and maintained by Klarälvdalens Datakonsult AB (KDAB).

The KDAB Group is the global No.1 software consultancy for Qt, C++ and
OpenGL applications across desktop, embedded and mobile platforms.

The KDAB Group provides consulting and mentoring for developing Qt applications
from scratch and in porting from all popular and legacy frameworks to Qt.
We continue to help develop parts of Qt and are one of the major contributors
to the Qt Project. We can give advanced or standard trainings anywhere
around the globe on Qt as well as C++, OpenGL, 3D and more.

Please visit <https://www.kdab.com> to meet the people who write code like this.

Stay up-to-date with KDAB product announcements:

- [KDAB Newsletter](https://news.kdab.com)
- [KDAB Blogs](https://www.kdab.com/category/blogs)
- [KDAB on Twitter](https://twitter.com/KDABQt)
