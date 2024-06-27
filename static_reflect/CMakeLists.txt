project(static_reflect)

set(HEADER_FILES
        declarative_struct.h)

add_library(cs_static_reflect INTERFACE)
target_include_directories(cs_static_reflect INTERFACE .)