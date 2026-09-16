

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
    file(GLOB GRAPHVIZ_PROGRAMW6432_ROOTS "$ENV{ProgramW6432}/Graphviz*")
    file(GLOB GRAPHVIZ_PROGRAMFILES_ROOTS "$ENV{ProgramFiles}/Graphviz*")
    list(APPEND GRAPHVIZ_SEARCH_ROOTS
        ${GRAPHVIZ_PROGRAMW6432_ROOTS}
        ${GRAPHVIZ_PROGRAMFILES_ROOTS}
    )
    list(REMOVE_DUPLICATES GRAPHVIZ_SEARCH_ROOTS)

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

    function(dcfilter_find_graphviz_library OUTVAR BASENAME)
        find_library(${OUTVAR}
            NAMES ${BASENAME}
            HINTS ${GRAPHVIZ_SEARCH_ROOTS}
            PATH_SUFFIXES lib lib/graphviz
        )
        if (${OUTVAR})
            set(${OUTVAR} "${${OUTVAR}}" PARENT_SCOPE)
            return()
        endif()

        unset(${OUTVAR} CACHE)
        unset(${OUTVAR})
        set(GRAPHVIZ_LIBRARY_NAMES)
        foreach(GRAPHVIZ_LIBRARY_VERSION RANGE 1 99)
            list(PREPEND GRAPHVIZ_LIBRARY_NAMES "${BASENAME}-${GRAPHVIZ_LIBRARY_VERSION}.lib")
        endforeach()

        find_file(${OUTVAR}
            NAMES ${GRAPHVIZ_LIBRARY_NAMES}
            HINTS ${GRAPHVIZ_SEARCH_ROOTS}
            PATH_SUFFIXES lib lib/graphviz
        )
        if (${OUTVAR})
            set(${OUTVAR} "${${OUTVAR}}" PARENT_SCOPE)
            return()
        endif()

        message(FATAL_ERROR "Could not find Graphviz ${BASENAME} library")
    endfunction()

    find_path(GRAPHVIZ_INCLUDE_DIR
        NAMES graphviz/cgraph.h
        HINTS ${GRAPHVIZ_SEARCH_ROOTS}
        PATH_SUFFIXES include
        REQUIRED
    )
    dcfilter_find_graphviz_library(GRAPHVIZ_CGRAPH_LIBRARY cgraph)
    dcfilter_find_graphviz_library(GRAPHVIZ_GVC_LIBRARY gvc)

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
