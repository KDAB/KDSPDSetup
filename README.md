<!--
This file is part of KDSpdSetup.

SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>

SPDX-License-Identifier: MIT

Contact KDAB at <info@kdab.com> for commercial licensing options.
-->

```text
            ▄────▄ •▄ ·▄▄▄▄  .▄▄ ·  ▄▄▄··▄▄▄▄  .▄▄ · ▄▄▄ .▄▄▄▄▄▄• ▄▌ ▄▄▄·
         ▄██─────█▌▄▌▪██▪ ██ ▐█ ▀. ▐█ ▄███▪ ██ ▐█ ▀. ▀▄.▀·•██  █▪██▌▐█ ▄█
   ▄▀██▀█▀█▀███▀─▐▀▀▄·▐█· ▐█▌▄▀▀▀█▄ ██▀·▐█· ▐█▌▄▀▀▀█▄▐▀▀▪▄ ▐█.▪█▌▐█▌ ██▀·
 ▀▀▀▀▀████▀▀▀────▐█.█▌██. ██ ▐█▄▪▐█▐█▪·•██. ██ ▐█▄▪▐█▐█▄▄▌ ▐█▌·▐█▄█▌▐█▪·•
       ▀██───────·▀  ▀▀▀▀▀▀•  ▀▀▀▀ .▀   ▀▀▀▀▀•  ▀▀▀▀  ▀▀▀  ▀▀▀  ▀▀▀ .▀
```

![MIT License](https://img.shields.io/badge/MIT_LICENSE-darkgreen?style=for-the-badge&logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIHZlcnNpb249IjEiIHdpZHRoPSIyNSIgaGVpZ2h0PSIyNSI+PHBhdGggZD0iTTEyLjc1IDIuNzVWNC41aDEuOTc1Yy4zNTEgMCAuNjk0LjEwNi45ODQuMzAzbDEuNjk3IDEuMTU0Yy4wNDEuMDI4LjA5LjA0My4xNC4wNDNoNC4xMDJhLjc1Ljc1IDAgMCAxIDAgMS41SDIwLjA3bDMuMzY2IDcuNjhhLjc0OS43NDkgMCAwIDEtLjIzLjg5NmMtLjEuMDc0LS4yMDMuMTQzLS4zMS4yMDZhNi4yOTYgNi4yOTYgMCAwIDEtLjc5LjM5OSA3LjM0OSA3LjM0OSAwIDAgMS0yLjg1Ni41NjkgNy4zNDMgNy4zNDMgMCAwIDEtMi44NTUtLjU2OCA2LjIwNSA2LjIwNSAwIDAgMS0uNzktLjQgMy4yMDUgMy4yMDUgMCAwIDEtLjMwNy0uMjAybC0uMDA1LS4wMDRhLjc0OS43NDkgMCAwIDEtLjIzLS44OTZsMy4zNjgtNy42OGgtLjg4NmMtLjM1MSAwLS42OTQtLjEwNi0uOTg0LS4zMDNsLTEuNjk3LTEuMTU0YS4yNDYuMjQ2IDAgMCAwLS4xNC0uMDQzSDEyLjc1djE0LjVoNC40ODdhLjc1Ljc1IDAgMCAxIDAgMS41SDYuNzYzYS43NS43NSAwIDAgMSAwLTEuNWg0LjQ4N1Y2SDkuMjc1YS4yNDkuMjQ5IDAgMCAwLS4xNC4wNDNMNy40MzkgNy4xOTdjLS4yOS4xOTctLjYzMy4zMDMtLjk4NC4zMDNoLS44ODZsMy4zNjggNy42OGEuNzUuNzUgMCAwIDEtLjIwOS44NzhjLS4wOC4wNjUtLjE2LjEyNi0uMzEuMjIzYTYuMDc3IDYuMDc3IDAgMCAxLS43OTIuNDMzIDYuOTI0IDYuOTI0IDAgMCAxLTIuODc2LjYyIDYuOTEzIDYuOTEzIDAgMCAxLTIuODc2LS42MiA2LjA3NyA2LjA3NyAwIDAgMS0uNzkyLS40MzMgMy40ODMgMy40ODMgMCAwIDEtLjMwOS0uMjIxLjc2Mi43NjIgMCAwIDEtLjIxLS44OEwzLjkzIDcuNUgyLjM1M2EuNzUuNzUgMCAwIDEgMC0xLjVoNC4xMDJjLjA1IDAgLjA5OS0uMDE1LjE0MS0uMDQzbDEuNjk1LTEuMTU0Yy4yOS0uMTk4LjYzNC0uMzAzLjk4NS0uMzAzaDEuOTc0VjIuNzVhLjc1Ljc1IDAgMCAxIDEuNSAwWk0yLjE5MyAxNS4xOThhNS40MTQgNS40MTQgMCAwIDAgMi41NTcuNjM1IDUuNDE0IDUuNDE0IDAgMCAwIDIuNTU3LS42MzVMNC43NSA5LjM2OFptMTQuNTEtLjAyNGMuMDgyLjA0LjE3NC4wODMuMjc1LjEyNi41My4yMjMgMS4zMDUuNDUgMi4yNzIuNDVhNS44NDcgNS44NDcgMCAwIDAgMi41NDctLjU3NkwxOS4yNSA5LjM2N1oiIGZpbGw9IiNmZmYiPjwvcGF0aD48L3N2Zz4=)![C++](https://img.shields.io/badge/c%2B%2B-red?style=for-the-badge&logo=c%2B%2B)![CMake](https://img.shields.io/badge/CMake-%23008FBA.svg?style=for-the-badge&logo=cmake&logoColor=white)![Git](https://img.shields.io/badge/git-%23F05033.svg?style=for-the-badge&logo=git&logoColor=white)![Linux](https://img.shields.io/badge/Linux-FCC624?style=for-the-badge&logo=linux&logoColor=black)![Windows](https://img.shields.io/badge/Windows-0078D6?style=for-the-badge&logo=windows&logoColor=white)![macOS](https://img.shields.io/badge/mac%20os-000000?style=for-the-badge&logo=macos&logoColor=F0F0F0)

# KDSPDSetup

## Overview

KDSPDSetup is a minimal reimplementation of the unmaintained [spdlog_setup](https://github.com/guangie88/spdlog_setup) library for modern C++ projects.

Given a TOML file containing configuration parameters, `spdlog` loggers for your project can be set up with a single call to `KDSPDSetup::setupFrom`, as shown below:

```cpp
#include <KDSpdSetup/kdspdsetup.h>

int main()
{
    KDSPDSetup::setupFrom("example.toml");

    auto logger = spdlog::get("root");
    logger->debug("Hello spdlog!");
}

// [2023-10-30 14:35:19.244] [root] [debug] Hello spdlog!

```

The configuration file for this example, `example.toml`, might look like this:

```c
[[sink]]
name = "console_st"
type = "stdout_sink_st"

[[logger]]
name = "root"
sinks = ["console_st"]
level = "debug"
```

## Requirements

You will need a compiler that supports the C++20 standard.

Our CI has successfully built and tested on these environments:
|Platform|Kernel|Arch|Compiler|Compiler Version
|-|-|-|-|-
| Windows 10 | Microsoft Windows [Version 10.0.19044.1526] | x64 | MSVC | 19.34.31937.0
| Ubuntu GNU/Linux | 4.15.0-167-generic #175-Ubuntu SMP | x64 | GCC | 11.4.0
| MacOS | Darwin Kernel Version 22.6.0 xnu-8796.141.3.701.17 | arm64 | clang | 14.0.3.14030022

The following compiler versions should be sufficient, but this has not been fully tested:

| ![GNU Badge](https://img.shields.io/badge/gcc-A42E2B?logo=gnu&logoColor=fff&style=for-the-badge) | ![LLVM Badge](https://img.shields.io/badge/clang-262D3A?logo=llvm&logoColor=fff&style=for-the-badge) | ![Visual Studio Badge](https://img.shields.io/badge/msvc-5C2D91?logo=visualstudio&logoColor=fff&style=for-the-badge) | ![NVIDIA Badge](https://img.shields.io/badge/nvcc-76B900?logo=nvidia&logoColor=fff&style=for-the-badge) | ![Intel Badge](https://img.shields.io/badge/Intel%20C++-0071C5?logo=intel&logoColor=fff&style=for-the-badge)
|-|-|-|-|-
| GCC 10+ | Clang 8+ | MSVC 19.22+ | NVCC 12.0+ | Intel C++ 2021.1+

## Dependencies

Aside from [spdlog](https://github.com/gabime/spdlog), there are three additional dependencies:

| Purpose       | Lib                                           | Needed
| ------------- | --------------------------------------------- | ----------
| TOML parsing  | [toml11](https://github.com/ToruNiina/toml11) | Always
| Testing       | [doctest](https://github.com/doctest/doctest) | Tests only
| Documentation | [doxygen](https://github.com/doxygen/doxygen) | Documentation only

These dependencies will be automatically downloaded if not found.

_** NOTE: doxygen is not automatically downloaded if not found yet **_

## Build & Install

### Build

_** NOTE: all the flags are still on by default and should probably be turned off **_

You'll need at least CMake 3.5 to build this library.

To configure and build, use the following `cmake` commands:

```bash
$ cmake -S <source-dir> -B <build-dir> <flags>
$ cmake --build <build-dir>
```

where `<source-dir>` is the root directory of this project and `<build-dir>` is the desired build directory.

`<flags>` can be empty or contain one or more of the flags detailed in the [Tests](#tests), [Examples](#examples), and [Documentation](#documentation) sections below.

For example, from this directory one might run the following:

```bash
$ cmake -S . -B build
$ cmake --build build
```

which creates the directory `build` if it does not exist, configures CMake, and builds the project.

### Install

To install the library on your system, after completing the steps in the [Build](#build) section above, run the following command:

```bash
$ sudo cmake --install <build-dir>
```

### Tests

You can build the unit tests by passing the flag `-DKDSPDSETUP_BUILD_TESTS` to CMake:

```bash
$ cmake -S <source-dir> -B <build-dir> -DKDSPDSETUP_BUILD_TESTS
$ cmake --build <build-dir>
```

The dependency [doctest](https://github.com/doctest/doctest) will only be used when this flag is on.

### Examples

To build the basic examples, pass the flag `-DKDSPDSETUP_BUILD_EXAMPLES` to CMake:

```bash
$ cmake -S <source-dir> -B <build-dir> -DKDSPDSETUP_BUILD_EXAMPLES
$ cmake --build <build-dir>
```

### Documentation

To build the documentation, pass the flag `-DKDSPDSETUP_BUILD_DOCS` to CMake.

```bash
$ cmake -S <source-dir> -B <build-dir> -DKDSPDSETUP_BUILD_DOCS
$ cmake --build <build-dir>
```

The dependency [doxygen](https://github.com/doxygen/doxygen) will only be used when this flag is on.

## Usage

### Use With CMake Project

To use the library in a CMake project, make sure to [install](#install) the library.

Then, simply write the following line in your `CMakeLists.txt`:

```cmake
find_package(KDSpdSetup)
```

and after adding your target, let's call it `untitled`, link with this CMake command:

```cmake
target_link_libraries(untitled KDSpdSetup::KDSpdSetup)
```

### Configuration

Writing configuration files that can be used by KDSPDSetup is simple.

#### Sinks

To configure a sink, a name and sink type must be provided:

```c
[[sink]]
name = "some_name"
type = "stdout_sink_st"
```

Optionally, a level can also be specified:

```c
[[sink]]
name = "stdout_error"
type = "stdout_sink_mt"
level = "err"
```

Some sink types require additional fields, or allow additional optional fields.

The list below fully details these:

|`type`|required field|value type
|-|-|-
|`basic_file_sink_st`<br>`basic_file_sink_mt` |`filename`|`string`
|`rotating_file_sink_st`<br>`rotating_file_sink_mt`|`base_filename`<br>`max_size`<br>`max_files`|`string`<br>`string`<br>`int`
|`daily_file_sink_st`<br>`daily_file_sink_mt`|`base_filename`<br>`rotation_hour`<br>`rotation_minute`|`string`<br>`int`<br>`int`

The following sink types can specify the boolean `truncate`, which defaults to `false` if not specified.

- `basic_file_sink_st`<br>`basic_file_sink_mt`
- `rotating_file_sink_st`<br>`rotating_file_sink_mt`
- `daily_file_sink_st`<br>`daily_file_sink_mt`

The syslog sink types, `syslog_sink_st` and `syslog_sink_mt`, have four optional fields that generally do not need to be filled (but can be if needed):
|key name|value type|default value
|-|-|-
|`ident`|`string`|`""`
|`syslog_option`|`int`|`0`
|`syslog_facility`|`int`|`1`

Note that a `syslog_facility` value of `1` is the plain integer value for `LOG_USER`.

Here is a full list of currently supported sink types and the platforms on which they are available:

|Sink Type|Platform(s)
|-|-
|`stdout_sink_st`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`stdout_sink_mt`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`basic_file_sink_st`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`basic_file_sink_mt`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`rotating_file_sink_st`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`rotating_file_sink_mt`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`daily_file_sink_st`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`daily_file_sink_mt`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`null_sink_st`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`null_sink_mt`|![All](https://img.shields.io/badge/all-darkgreen?logo=data:image/svg+xml;base64,PHN2ZyB4bWxucz0iaHR0cDovL3d3dy53My5vcmcvMjAwMC9zdmciIGhlaWdodD0iMTYiIHdpZHRoPSIxNCIgdmlld0JveD0iMCAwIDQ0OCA1MTIiPjwhLS0hRm9udCBBd2Vzb21lIEZyZWUgNi41LjEgYnkgQGZvbnRhd2Vzb21lIC0gaHR0cHM6Ly9mb250YXdlc29tZS5jb20gTGljZW5zZSAtIGh0dHBzOi8vZm9udGF3ZXNvbWUuY29tL2xpY2Vuc2UvZnJlZSBDb3B5cmlnaHQgMjAyMyBGb250aWNvbnMsIEluYy4tLT48cGF0aCBvcGFjaXR5PSIxIiBmaWxsPSJ3aGl0ZSIgZD0iTTQzOC42IDEwNS40YzEyLjUgMTIuNSAxMi41IDMyLjggMCA0NS4zbC0yNTYgMjU2Yy0xMi41IDEyLjUtMzIuOCAxMi41LTQ1LjMgMGwtMTI4LTEyOGMtMTIuNS0xMi41LTEyLjUtMzIuOCAwLTQ1LjNzMzIuOC0xMi41IDQ1LjMgMEwxNjAgMzM4LjcgMzkzLjQgMTA1LjRjMTIuNS0xMi41IDMyLjgtMTIuNSA0NS4zIDB6Ii8+PC9zdmc+&style=for-the-badge)
|`syslog_sink_st`|![Linux](https://img.shields.io/badge/Linux_Only-FCC624?style=for-the-badge&logo=linux&logoColor=black)
|`syslog_sink_mt`|![Linux](https://img.shields.io/badge/Linux_Only-FCC624?style=for-the-badge&logo=linux&logoColor=black)
|`msvc_sink_st`|![Windows](https://img.shields.io/badge/Windows_Only-0078D6?style=for-the-badge&logo=windows&logoColor=white)
|`msvc_sink_mt`|![Windows](https://img.shields.io/badge/Windows_Only-0078D6?style=for-the-badge&logo=windows&logoColor=white)

Please note that the suffix `_st` refers to single thread, and `_mt` to multiple threads.

_** NOTE: `color_stdout_sink_st` and `color_stdout_sink_mt` are not yet supported, I never actually finished implementing them... this note is kind of a reminder for me to do that **_

#### Patterns

The global pattern is defined as a free-standing key-value pair (not under a `[[pattern]]` or any other table):

```c
global_pattern = "[%Y-%m-%dT%T%z] [%L] <%n>: %v"
```

If `global_pattern` is not specified, the spdlog uses the default pattern `“[%Y-%m-%d %H:%M:%S. %e] [%l] [%n] %v”`.

Additionally, named patterns can be defined and reused with several different loggers:

```c
[[pattern]]
name = "short_pattern"
value = "%c-%L: %v"
```

#### Thread Pools

An async logger takes a thread pool as an argument to its constructor. This can be either a global thread pool, or a manually constructed one.

The global thread pool is defined as a table with single brackets (`[global_thread_pool]`, **not** `[[global_thread_pool]]`) because it is one table, not an array of tables. Named thread pools are members of the array of tables `[[threadpool]]`.

Both require integers `queue_size` and `num_threads`, while non-global thread pools also require a name.

```c
[global_thread_pool]
queue_size = 8192
num_threads = 1

[[thread_pool]]
name = "tp"
queue_size = 4096
num_threads = 2
```

#### Loggers

Loggers require a name, and a list of sink names:

```c
[[logger]]
name = "my_logger"
sinks = ["console_sink_st", "console_sink_mt", "file_out", "file_err"]
```

Please keep in mind that each sink name must be a `name` from a `[[sink]]` defined somewhere in the configuration file. If the name is not found, `std::out_of_range` will be thrown.

Loggers can also optionally specify patterns and levels:

```c
[[logger]]
name = "root"
sinks = [
    "console_st", "console_mt",
    "color_console_st", "color_console_mt",
    "daily_out", "daily_err",
    "file_out", "file_err",
    "rotate_out", "rotate_err",
    "null_sink_st", "null_sink_mt",
    "syslog_st", "syslog_mt"]
level = "trace"
pattern = "short_pattern"
```

The pattern name also must be defined somewhere in the configuration.

Asynchronous loggers can be defined by specifying `type = "async"`:

```c
[[logger]]
type = "async"
name = "my_async"
sinks = ["console_mt"]
```

Optionally, async loggers can have a specified thread pool and overflow policy:

```c
[[logger]]
type = "async"
name = "local_async"
sinks = ["console_mt"]
pattern = "succient"
thread_pool = "tp"
overflow_policy = "overrun_oldest"
```

If no thread pool is specified, the global thread pool is used.

The options for overflow policy are `"block"` and `"overrun_oldest"`. If not specified, `"block"` is used by default.

#### Example Files

This repository contains a few example configuration files:

- `examples/example.toml`
- `tests/full/full-linux.toml`
- `tests/full/full-linux.toml`
- `tests/pre/pre.toml`

Huge thanks to [guangie88](https://github.com/guangie88) for providing these examples in the original [spdlog_setup](https://github.com/guangie88/spdlog_setup) project. The files in that project are available under the terms of the [MIT](https://github.com/guangie88/spdlog_setup/blob/master/LICENSE) license.

## Licensing

KDSPDSetup is (C) 2023, Klarälvdalens Datakonsult AB, and is available under the terms of the [MIT](LICENSE.txt) license.

Contact KDAB at <info@kdab.com> if you need different licensing options.

_** NOTE: the MIT link above does not work in the Doxygen docs because it links to LICENSE.txt, which can't be found from the built docs directory. Once this is a public repo, the url for the license file should be put there instead **_

## About KDAB

KDSPDSetup is supported and maintained by Klarälvdalens Datakonsult AB (KDAB).

The KDAB Group is the global No.1 software consultancy for Qt, C++ and OpenGL applications across desktop, embedded and mobile platforms.

The KDAB Group provides consulting and mentoring for developing Qt applications from scratch and in porting from all popular and legacy frameworks to Qt. We continue to help develop parts of Qt and are one of the major contributors to the Qt Project. We can give advanced or standard trainings anywhere around the globe on Qt as well as C++, OpenGL, 3D and more.

Please visit <https://www.kdab.com> to meet the people who write code like this.

Stay up-to-date with KDAB product announcements:

- [KDAB Newsletter](https://news.kdab.com)
- [KDAB Blogs](https://www.kdab.com/category/blogs)
- [KDAB on Twitter](https://twitter.com/KDABQt)
