project(container)

set(HEADER_FILES
        falgo.h
        safe_vector.h
        sync_pool.h)

set(SOURCE_FILES falgo.cpp)

add_library(container_lib ${SOURCE_FILES} ${HEADER_FILES})