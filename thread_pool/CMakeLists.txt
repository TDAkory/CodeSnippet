project(thread_pool)

set(HEADER_FILES
        safe_queue.h
        thread_pool.h)

set(LIB_SOURCE_FILES
)

set(EXE_SOURCE_FILES
        example.cpp)

add_library(cs_thread_pool INTERFACE)
target_link_libraries(cs_thread_pool INTERFACE ${HEADER_FILES})
target_include_directories(cs_thread_pool INTERFACE .)

add_executable(ThreadPool_run ${EXE_SOURCE_FILES} ${HEADER_FILES})