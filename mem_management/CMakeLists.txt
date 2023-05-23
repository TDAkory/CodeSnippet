project(mem_management)

set(HEADER_FILES sync_pool.h)

add_library(mem_management_lib INTERFACE)

target_include_directories(mem_management_lib INTERFACE .)
