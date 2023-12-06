[//]: <> ( This file is part of KDSpdSetup.                                        )
[//]: <> (                                                                         )
[//]: <> ( SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group )
[//]: <> ( company <info@kdab.com>                                                 )
[//]: <> (                                                                         )
[//]: <> ( SPDX-License-Identifier: MIT                                            )
[//]: <> (                                                                         )
[//]: <> ( Contact KDAB at <info@kdab.com> for commercial licensing options.       )
[//]: <> (                                                                         )
<div align=center>

```
            ▄────▄ •▄ ·▄▄▄▄  .▄▄ ·  ▄▄▄··▄▄▄▄  .▄▄ · ▄▄▄ .▄▄▄▄▄▄• ▄▌ ▄▄▄·
         ▄██─────█▌▄▌▪██▪ ██ ▐█ ▀. ▐█ ▄███▪ ██ ▐█ ▀. ▀▄.▀·•██  █▪██▌▐█ ▄█
   ▄▀██▀█▀█▀███▀─▐▀▀▄·▐█· ▐█▌▄▀▀▀█▄ ██▀·▐█· ▐█▌▄▀▀▀█▄▐▀▀▪▄ ▐█.▪█▌▐█▌ ██▀·
 ▀▀▀▀▀████▀▀▀────▐█.█▌██. ██ ▐█▄▪▐█▐█▪·•██. ██ ▐█▄▪▐█▐█▄▄▌ ▐█▌·▐█▄█▌▐█▪·•
       ▀██───────·▀  ▀▀▀▀▀▀•  ▀▀▀▀ .▀   ▀▀▀▀▀•  ▀▀▀▀  ▀▀▀  ▀▀▀  ▀▀▀ .▀
```

![MIT License](https://img.shields.io/badge/MIT_LICENSE-blue?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZlcnNpb249IjEiIHdpZHRoPSIyNSIgaGVpZ2h0PSIyNSI+PHBhdGggZD0iTTEyLjc1IDIuNzVWNC41aDEuOTc1Yy4zNTEgMCAuNjk0LjEwNi45ODQuMzAzbDEuNjk3IDEuMTU0Yy4wNDEuMDI4LjA5LjA0My4xNC4wNDNoNC4xMDJhLjc1Ljc1IDAgMCAxIDAgMS41SDIwLjA3bDMuMzY2IDcuNjhhLjc0OS43NDkgMCAwIDEtLjIzLjg5NmMtLjEuMDc0LS4yMDMuMTQzLS4zMS4yMDZhNi4yOTYgNi4yOTYgMCAwIDEtLjc5LjM5OSA3LjM0OSA3LjM0OSAwIDAgMS0yLjg1Ni41NjkgNy4zNDMgNy4zNDMgMCAwIDEtMi44NTUtLjU2OCA2LjIwNSA2LjIwNSAwIDAgMS0uNzktLjQgMy4yMDUgMy4yMDUgMCAwIDEtLjMwNy0uMjAybC0uMDA1LS4wMDRhLjc0OS43NDkgMCAwIDEtLjIzLS44OTZsMy4zNjgtNy42OGgtLjg4NmMtLjM1MSAwLS42OTQtLjEwNi0uOTg0LS4zMDNsLTEuNjk3LTEuMTU0YS4yNDYuMjQ2IDAgMCAwLS4xNC0uMDQzSDEyLjc1djE0LjVoNC40ODdhLjc1Ljc1IDAgMCAxIDAgMS41SDYuNzYzYS43NS43NSAwIDAgMSAwLTEuNWg0LjQ4N1Y2SDkuMjc1YS4yNDkuMjQ5IDAgMCAwLS4xNC4wNDNMNy40MzkgNy4xOTdjLS4yOS4xOTctLjYzMy4zMDMtLjk4NC4zMDNoLS44ODZsMy4zNjggNy42OGEuNzUuNzUgMCAwIDEtLjIwOS44NzhjLS4wOC4wNjUtLjE2LjEyNi0uMzEuMjIzYTYuMDc3IDYuMDc3IDAgMCAxLS43OTIuNDMzIDYuOTI0IDYuOTI0IDAgMCAxLTIuODc2LjYyIDYuOTEzIDYuOTEzIDAgMCAxLTIuODc2LS42MiA2LjA3NyA2LjA3NyAwIDAgMS0uNzkyLS40MzMgMy40ODMgMy40ODMgMCAwIDEtLjMwOS0uMjIxLjc2Mi43NjIgMCAwIDEtLjIxLS44OEwzLjkzIDcuNUgyLjM1M2EuNzUuNzUgMCAwIDEgMC0xLjVoNC4xMDJjLjA1IDAgLjA5OS0uMDE1LjE0MS0uMDQzbDEuNjk1LTEuMTU0Yy4yOS0uMTk4LjYzNC0uMzAzLjk4NS0uMzAzaDEuOTc0VjIuNzVhLjc1Ljc1IDAgMCAxIDEuNSAwWk0yLjE5MyAxNS4xOThhNS40MTQgNS40MTQgMCAwIDAgMi41NTcuNjM1IDUuNDE0IDUuNDE0IDAgMCAwIDIuNTU3LS42MzVMNC43NSA5LjM2OFptMTQuNTEtLjAyNGMuMDgyLjA0LjE3NC4wODMuMjc1LjEyNi41My4yMjMgMS4zMDUuNDUgMi4yNzIuNDVhNS44NDcgNS44NDcgMCAwIDAgMi41NDctLjU3NkwxOS4yNSA5LjM2N1oiIGZpbGw9IiNmZmYiPjwvcGF0aD48L3N2Zz4=)
![C++](https://img.shields.io/badge/c%2B%2B-red?style=for-the-badge&logo=c%2B%2B)
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
call to `KDSPDSetup::setupFrom`, as shown below:

```cpp
#include <kdspdsetup.h>

int main()
{
    KDSPDSetup::setupFrom("example.toml");

    auto logger = spdlog::get("root");
    logger->debug("Hello spdlog!");
}

// [2023-10-30 14:35:19.244] [root] [debug] Hello spdlog!

```

## Requirements

You will need a compiler that supports the C++20 standard.

The following should be sufficient (still needs testing):

| ![GNU Badge](https://img.shields.io/badge/gcc-A42E2B?logo=gnu&logoColor=fff&style=for-the-badge) | ![LLVM Badge](https://img.shields.io/badge/clang-262D3A?logo=llvm&logoColor=fff&style=for-the-badge) | ![Visual Studio Badge](https://img.shields.io/badge/msvc-5C2D91?logo=visualstudio&logoColor=fff&style=for-the-badge) | ![NVIDIA Badge](https://img.shields.io/badge/nvcc-76B900?logo=nvidia&logoColor=fff&style=for-the-badge) | ![Intel Badge](https://img.shields.io/badge/Intel%20C++-0071C5?logo=intel&logoColor=fff&style=for-the-badge) |
| ------------------------------------------------------------------------------------------------ | ---------------------------------------------------------------------------------------------------- | -------------------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------- | ------------------------------------------------------------------------------------------------------------ |
| GCC 10+                                                                                          | Clang 8+                                                                                             | MSVC 19.22+                                                                                                          | NVCC 12.0+                                                                                              | Intel C++ 2021.1+                                                                                            |

## Dependencies

Aside from the obvious dependencies [spdlog](https://github.com/gabime/spdlog) and
[fmt](https://github.com/fmtlib/fmt), there are two additional dependencies:

| Purpose      | Lib                                           | Needed     |
| ------------ | --------------------------------------------- | ---------- |
| TOML parsing | [toml11](https://github.com/ToruNiina/toml11) | Always     |
| Testing      | [doctest](https://github.com/doctest/doctest) | Tests only |

These dependencies will be automatically downloaded if not found.

## Build & Install

### Build

You'll need at least CMake 3.5 to build this library.

... ... ...

### Install

Install <...> (WIP)

### Tests

You can build the unit tests by passing the flag `-DKDSPDSETUP_BUILD_TESTS` to CMake. The dependency [doctest](https://github.com/doctest/doctest)
will only be used when this flag is on.

### Examples

To build the basic examples, pass the flag `-DKDSPDSETUP_BUILD_EXAMPLES` to CMake.

## Configuration

How to write config TOML file

## Licensing

KDSPDSetup is (C) 2023, Klarälvdalens Datakonsult AB, and is available under the
terms of the [MIT](LICENSE.txt) license.

Contact KDAB at <info@kdab.com> if you need different licensing options.

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
