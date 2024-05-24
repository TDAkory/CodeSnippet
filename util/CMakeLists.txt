project(util)

set(HEADER_FILES simple_log.h)

set(SRC_FILES simple_log.cpp)

add_library(cs_util ${SRC_FILES})
target_include_directories(cs_util INTERFACE .)