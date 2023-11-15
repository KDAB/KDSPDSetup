function(add_kdspdsetup_test NAME SOURCES)
    set(TARGET_NAME test_kdspdsetup_${NAME})
    add_executable(${TARGET_NAME} ${SOURCES})

    target_link_libraries(${TARGET_NAME} kdspdsetup doctest)

    add_test(NAME ${TARGET_NAME} COMMAND $<TARGET_FILE:${TARGET_NAME}>)
    set_tests_properties(${TARGET_NAME} PROPERTIES LABELS "kdspdsetup")

    if(APPLE)
        target_compile_options(${TARGET_NAME} PRIVATE -Wno-deprecated-declarations)
    endif()
endfunction()

add_subdirectory(full)
add_subdirectory(pre)
add_subdirectory(internals)
add_subdirectory(malformed)
