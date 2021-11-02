
file(GLOB_RECURSE SRC_FILES *.cpp)

add_library(rules ${SRC_FILES})
target_link_libraries(rules PUBLIC base)
target_include_directories(rules PUBLIC ${PROJECT_SOURCE_DIR}/base)
