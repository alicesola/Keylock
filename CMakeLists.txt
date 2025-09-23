cmake_minimum_required(VERSION 3.16)

project(Keylock)
set(CMAKE_CXX_STANDARD 20)

add_executable(Keylock
    main.cpp
    ui/repl.cpp
    data/dataIO.cpp
)

target_include_directories(Keylock PRIVATE 
${CMAKE_CURRENT_SOURCE_DIR}/data
${CMAKE_CURRENT_SOURCE_DIR}/ui
)
find_package(nlohmann_json CONFIG REQUIRED)   # 找到 vcpkg 里的 nlohmann-json
target_link_libraries(Keylock PRIVATE nlohmann_json::nlohmann_json)
