project(test-full LANGUAGES CXX)

configure_file(full.toml full.toml)
add_kdspdsetup_test(${PROJECT_NAME} tst_full.cpp)
