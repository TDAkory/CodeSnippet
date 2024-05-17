project(util)

set(HEADER_FILES simple_log.h)

set(SRC_FILES simple_log.cpp)

add_library(Util_lib ${SRC_FILES})

target_include_directories(Util_lib INTERFACE .)