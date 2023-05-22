project(mem_management)

set(HEADER_FILES
        sync_pool.h)
set(SOURCE_FILES
        sync_pool.cpp)

add_library(mem_management_lib ${SOURCE_FILES} ${HEADER_FILES})