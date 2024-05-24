project(str_ops)

set(HEADER_FILES
        StringSearch.h)
set(SOURCE_FILES
        StringSearch.cpp)

add_library(cs_str ${SOURCE_FILES} ${HEADER_FILES})