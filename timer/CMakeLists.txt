project(timer)

set(LIB_HEADER_FILES
        timer_base.h
        callback_timer.h
        timer_decorator.h)

set(LIB_SOURCE_FILES
        callback_timer.cpp)

add_library(cs_timer ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})

set(EXE_SOURCE_FILES
        timer_run.cpp)

add_executable(Timer_run ${EXE_SOURCE_FILES})
target_link_libraries(Timer_run PUBLIC timer_lib)
target_include_directories(Timer_run PUBLIC .)