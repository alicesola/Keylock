cmake_minimum_required(VERSION 3.16)

project(Keylock)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_executable(Keylock
    Cli_main.cpp
    ui/repl.cpp
    data/dataIO.cpp
    security/crypto.cpp
)

target_include_directories(Keylock PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/data
${CMAKE_CURRENT_SOURCE_DIR}/ui
${CMAKE_CURRENT_SOURCE_DIR}/security
)
find_package(nlohmann_json CONFIG REQUIRED)   # 找到 vcpkg 里的 nlohmann-json
find_package(OpenSSL CONFIG REQUIRED)

find_library(ARGON2_LIB argon2 REQUIRED)

target_link_libraries(Keylock PRIVATE 
    nlohmann_json::nlohmann_json
    OpenSSL::SSL 
    OpenSSL::Crypto 
    ${ARGON2_LIB}
)
