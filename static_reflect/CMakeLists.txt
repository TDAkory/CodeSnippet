project(StaticReflect)

set(HEADER_FILES
        declarative_struct.h)

add_library(StaticReflect INTERFACE ${HEADER_FILES})

target_include_directories(StaticReflect INTERFACE .)
