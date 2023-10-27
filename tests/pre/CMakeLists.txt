project(test-pre LANGUAGES CXX)

configure_file(pre.toml pre.toml)
add_kdspdsetup_test(${PROJECT_NAME} tst_pre.cpp)
