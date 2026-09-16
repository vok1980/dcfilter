

add_subdirectory(bit_array)

add_executable(dcfilter)

target_sources(dcfilter
    PRIVATE
        dcfilter.c
)

set(DCFILTER_GRAPHVIZ_LIBRARIES
    cgraph
    gvc
)

if (MSVC)
    set(GRAPHVIZ_ROOT "" CACHE PATH "Graphviz installation root")

    if (NOT GRAPHVIZ_ROOT)
        find_program(GRAPHVIZ_DOT_EXECUTABLE
            NAMES dot
            REQUIRED
        )
        get_filename_component(GRAPHVIZ_BIN_DIR "${GRAPHVIZ_DOT_EXECUTABLE}" DIRECTORY)
        get_filename_component(GRAPHVIZ_ROOT "${GRAPHVIZ_BIN_DIR}" DIRECTORY)
    endif()

    find_path(GRAPHVIZ_INCLUDE_DIR
        NAMES graphviz/cgraph.h
        PATHS ${GRAPHVIZ_ROOT}/include
        REQUIRED
    )
    find_library(GRAPHVIZ_CGRAPH_LIBRARY
        NAMES cgraph
        PATHS ${GRAPHVIZ_ROOT}/lib
        REQUIRED
    )
    find_library(GRAPHVIZ_GVC_LIBRARY
        NAMES gvc
        PATHS ${GRAPHVIZ_ROOT}/lib
        REQUIRED
    )

    target_sources(dcfilter
        PRIVATE
            compat/getopt.c
    )
    target_include_directories(dcfilter
        PRIVATE
            compat
            ${GRAPHVIZ_INCLUDE_DIR}
    )

    set(DCFILTER_GRAPHVIZ_LIBRARIES
        ${GRAPHVIZ_CGRAPH_LIBRARY}
        ${GRAPHVIZ_GVC_LIBRARY}
    )
endif()

# libgraphviz-dev
target_link_libraries(dcfilter
    PUBLIC
        bit_array
        ${DCFILTER_GRAPHVIZ_LIBRARIES}
)

install(
    TARGETS dcfilter
    DESTINATION bin
)
