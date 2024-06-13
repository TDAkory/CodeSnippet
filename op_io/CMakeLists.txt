project(io_op)

set(HEADER_FILES)

set(SRC_FILES)

# add_library(cs_io ${SRC_FILES} ${HEADER_FILES})

add_executable(io_run main.cpp)
add_executable(uring_cat uring_cat.cpp)