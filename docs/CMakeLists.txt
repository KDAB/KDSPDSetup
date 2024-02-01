# This file is part of KDSpdSetup.
#
# SPDX-FileCopyrightText: 2023 Klarälvdalens Datakonsult AB, a KDAB Group company <info@kdab.com>
#
# SPDX-License-Identifier: MIT
#
# Contact KDAB at <info@kdab.com> for commercial licensing options.
#
find_package(Doxygen REQUIRED dot OPTIONAL_COMPONENTS mscgen dia)

if(DOXYGEN_FOUND)
    # set input and output files
    set(DOXYGEN_INPUT_DIR_LIST ${PROJECT_SOURCE_DIR} ${PROJECT_SOURCE_DIR}/src)
    set(DOXYGEN_OUTPUT_DIR ${PROJECT_BINARY_DIR}/docs)
    set(DOXYGEN_INDEX_FILE ${DOXYGEN_OUTPUT_DIR}/html/index.html)

    set(DOXYGEN_IN ${PROJECT_SOURCE_DIR}/docs/Doxyfile.in)
    set(DOXYGEN_OUT ${DOXYGEN_OUTPUT_DIR}/Doxyfile)

    configure_file(${DOXYGEN_IN} ${DOXYGEN_OUT} @ONLY)
    configure_file(doxygen-awesome.css doxygen-awesome.css)

    # Command for generating doc from Doxygen config file.
    add_custom_command(
        OUTPUT ${DOXYGEN_INDEX_FILE}
        COMMAND ${DOXYGEN_EXECUTABLE} ${DOXYGEN_OUT}
        MAIN_DEPENDENCY ${DOXYGEN_OUT}
        ${DOXYGEN_IN}
        COMMENT "Generating Doxygen documentation"
        VERBATIM
    )

    add_custom_target(docs ALL DEPENDS ${DOXYGEN_INDEX_FILE})
endif()
