project(example LANGUAGES CXX)
configure_file(example.toml example.toml)
add_executable(example main.cpp)
target_link_libraries(example KDSpdSetup::KDSpdSetup)
