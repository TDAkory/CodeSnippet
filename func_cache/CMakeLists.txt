project(func_cache)

set(HEADER_FILES
        *.h)
set(SOURCE_FILES
        *.cpp)

add_library(func_cache_lib ${SOURCE_FILES} ${HEADER_FILES})