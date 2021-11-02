
cmake_minimum_required(VERSION 3.10)
project(PartyQuirks CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

add_compile_options(-fdiagnostics-color=always)

add_subdirectory(base)
add_subdirectory(rules)

add_executable(pquirks main.cpp)

target_link_libraries(pquirks PUBLIC base)
target_link_libraries(pquirks PUBLIC rules)

target_include_directories(pquirks PUBLIC "${PROJECT_SOURCE_DIR}/base")
target_include_directories(pquirks PUBLIC "${PROJECT_SOURCE_DIR}/rules")


