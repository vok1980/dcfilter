
add_executable(dcfilter)

target_sources(dcfilter
    PRIVATE
        dcfilter.c
)

# libgraphviz-dev
target_link_libraries(dcfilter
    PUBLIC
        cgraph
        gvc
)
