project(container)

set(HEADER_FILES
        falgo.h
        safe_vector.h
        sync_pool.h binary_tree.cpp binary_tree.h)

set(SOURCE_FILES falgo.cpp binary_tree.cpp binary_tree.h)

add_library(container_lib ${SOURCE_FILES} ${HEADER_FILES} binary_tree.cpp binary_tree.h)