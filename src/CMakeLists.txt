

add_subdirectory(bit_array)

add_executable(dcfilter)

target_sources(dcfilter
    PRIVATE
        dcfilter.c
)

# libgraphviz-dev
target_link_libraries(dcfilter
    PUBLIC
        bit_array
        cgraph
        gvc
)
