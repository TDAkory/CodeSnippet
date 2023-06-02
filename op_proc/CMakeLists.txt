project(ProcOps)

set(HEADER_FILES
        proc_util.h)

set(SOURCE_FILES
        proc_util.cpp)

add_library(ProcOps_lib ${SOURCE_FILES} ${HEADER_FILES})