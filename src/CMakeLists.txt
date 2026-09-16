

add_subdirectory(bit_array)

add_executable(dcfilter)

target_sources(dcfilter
    PRIVATE
        dcfilter.c
)

if (MSVC)
    set(GRAPHVIZ_ROOT "" CACHE PATH "Graphviz installation root")

    target_sources(dcfilter
        PRIVATE
            compat/getopt.c
    )
    target_include_directories(dcfilter
        PRIVATE
            compat
            ${GRAPHVIZ_ROOT}/include
    )
    target_link_directories(dcfilter
        PRIVATE
            ${GRAPHVIZ_ROOT}/lib
    )
endif()

# libgraphviz-dev
target_link_libraries(dcfilter
    PUBLIC
        bit_array
        cgraph
        gvc
)

install(
    TARGETS dcfilter
    DESTINATION bin
)
