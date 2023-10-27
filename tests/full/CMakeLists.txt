project(test-full LANGUAGES CXX)

configure_file(full-win32.toml full-win32.toml)
configure_file(full-linux.toml full-linux.toml)
add_kdspdsetup_test(${PROJECT_NAME} tst_full.cpp)
