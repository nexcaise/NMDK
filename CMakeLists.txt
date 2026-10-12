cmake_minimum_required(VERSION 3.22)

project(NMDK LANGUAGES CXX)

add_library(nmdk SHARED IMPORTED GLOBAL)

set_target_properties(nmdk PROPERTIES
    IMPORTED_LOCATION "${CMAKE_CURRENT_LIST_DIR}/nmdk/libs/arm64-v8a/libnmdk.so"
    INTERFACE_INCLUDE_DIRECTORIES "${CMAKE_CURRENT_LIST_DIR}/nmdk/include"
)

add_library(NMDK::NMDK ALIAS nmdk)