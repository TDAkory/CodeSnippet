project(Timer)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(LIB_HEADER_FILES
        TimerBase.h
        CBTimer.h)

set(LIB_SOURCE_FILES
        CBTimer.cpp)

add_library(timer_lib ${LIB_SOURCE_FILES} ${LIB_HEADER_FILES})

set(EXE_SOURCE_FILES
        timer_run.cpp)

add_executable(timer_run ${EXE_SOURCE_FILES})

target_link_libraries(timer_run PUBLIC timer_lib)

target_include_directories(timer_run PUBLIC .)