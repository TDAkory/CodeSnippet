project(StringOps)

set(HEADER_FILES
        StringSearch.h)
set(SOURCE_FILES
        StringSearch.cpp)

add_library(StringOps_lib ${SOURCE_FILES} ${HEADER_FILES})