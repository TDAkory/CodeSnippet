project(func_ops)

set(HEADER_FILES
        func_cache.h
        scope_guard.h
        func_decerator.h
)

set(SRC_FILES scope_guard.cpp)

add_library(cs_func ${SRC_FILES} ${HEADER_FILES})