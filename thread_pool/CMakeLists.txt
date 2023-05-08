project(ThreadPool)

set(HEADER_FILES
        SafeQueue.h
        ThreadPool.h)

set(LIB_SOURCE_FILES
        )

set(EXE_SOURCE_FILES
        example.cpp)

add_library(ThreadPool_lib INTERFACE)
target_link_libraries(ThreadPool_lib INTERFACE ${HEADER_FILES})
target_include_directories(ThreadPool_lib INTERFACE .)

add_executable(ThreadPool_run ${EXE_SOURCE_FILES} ${HEADER_FILES})