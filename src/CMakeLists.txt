

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
    set(GRAPHVIZ_SEARCH_ROOTS
        "$ENV{ProgramW6432}/Graphviz"
        "$ENV{ProgramFiles}/Graphviz"
    )

    if (GRAPHVIZ_ROOT)
        list(PREPEND GRAPHVIZ_SEARCH_ROOTS "${GRAPHVIZ_ROOT}")
    else()
        find_program(GRAPHVIZ_DOT_EXECUTABLE
            NAMES dot
        )
        if (GRAPHVIZ_DOT_EXECUTABLE)
            get_filename_component(GRAPHVIZ_BIN_DIR "${GRAPHVIZ_DOT_EXECUTABLE}" DIRECTORY)
            get_filename_component(GRAPHVIZ_ROOT "${GRAPHVIZ_BIN_DIR}" DIRECTORY)
            list(PREPEND GRAPHVIZ_SEARCH_ROOTS "${GRAPHVIZ_ROOT}")
        endif()
    endif()

    find_path(GRAPHVIZ_INCLUDE_DIR
        NAMES graphviz/cgraph.h
        HINTS ${GRAPHVIZ_SEARCH_ROOTS}
        PATH_SUFFIXES include
        REQUIRED
    )
    find_library(GRAPHVIZ_CGRAPH_LIBRARY
        NAMES cgraph libcgraph
        HINTS ${GRAPHVIZ_SEARCH_ROOTS}
        PATH_SUFFIXES lib lib/graphviz
        REQUIRED
    )
    find_library(GRAPHVIZ_GVC_LIBRARY
        NAMES gvc libgvc
        HINTS ${GRAPHVIZ_SEARCH_ROOTS}
        PATH_SUFFIXES lib lib/graphviz
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
