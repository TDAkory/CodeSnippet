project(func_cache)

set(HEADER_FILES func_cache.h scope_guard.h)

set(SRC_FILES scope_guard.cpp)

add_library(func_cache_lib ${SRC_FILES})

target_include_directories(func_cache_lib INTERFACE .)