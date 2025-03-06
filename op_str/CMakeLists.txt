project(cs_str)

set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_STANDARD 20)

set(HEADER_FILES
        string_search.h
        string_split.h
)

set(SOURCE_FILES
        string_search.cpp
        string_split.cpp
)

add_library(cs_str ${SOURCE_FILES} ${HEADER_FILES})