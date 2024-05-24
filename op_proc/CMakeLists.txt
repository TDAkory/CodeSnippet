project(proc_ops)

set(HEADER_FILES
        proc_util.h)

set(SOURCE_FILES
        proc_util.cpp)

add_library(cs_proc ${SOURCE_FILES} ${HEADER_FILES})