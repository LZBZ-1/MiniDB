# src/core/CMakeLists.txt
add_library(minidb_core INTERFACE)
target_include_directories(minidb_core INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})
add_subdirectory(types)
