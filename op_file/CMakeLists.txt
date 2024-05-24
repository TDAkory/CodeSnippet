project(file_ops)

set(HEADER_FILES
        FileOps.h)
set(SOURCE_FILES
        FileOps.cpp)

add_library(cs_file ${SOURCE_FILES} ${HEADER_FILES})
