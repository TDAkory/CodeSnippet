project(proc_ops)

set(HEADER_FILES
        proc_util.h)

set(SOURCE_FILES
        proc_util.cpp
)

add_library(cs_proc ${SOURCE_FILES} ${HEADER_FILES})

add_executable(new_process new_process.cpp)
add_executable(main_process main_process.cpp)