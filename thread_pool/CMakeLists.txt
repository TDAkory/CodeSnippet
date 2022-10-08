project(ThreadPool)

set(HEADER_FILES
        SafeQueue.h
        ThreadPool.h)
set(SOURCE_FILES
        example.cpp)

add_library(ThreadPool_lib ${SOURCE_FILES} ${HEADER_FILES})

add_executable(ThreadPool_run ${SOURCE_FILES} ${HEADER_FILES})