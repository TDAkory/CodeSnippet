project(container)

set(HEADER_FILES
        falgo.h
        safe_vector.h
        sync_pool.h
        bi_tree_printer.h
)

set(SOURCE_FILES
        falgo.cpp
        bi_tree_printer.cpp
)

add_library(cs_container ${SOURCE_FILES} ${HEADER_FILES})