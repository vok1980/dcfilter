

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

        set(GRAPHVIZ_VERSIONED_LIBRARY "")
        set(GRAPHVIZ_VERSIONED_LIBRARY_VERSION -1)
        foreach(GRAPHVIZ_SEARCH_ROOT IN LISTS GRAPHVIZ_SEARCH_ROOTS)
            foreach(GRAPHVIZ_LIBRARY_SUFFIX IN ITEMS lib lib/graphviz)
                file(GLOB GRAPHVIZ_LIBRARY_CANDIDATES
                    LIST_DIRECTORIES FALSE
                    "${GRAPHVIZ_SEARCH_ROOT}/${GRAPHVIZ_LIBRARY_SUFFIX}/${BASENAME}-*.lib"
                )
                foreach(GRAPHVIZ_LIBRARY_CANDIDATE IN LISTS GRAPHVIZ_LIBRARY_CANDIDATES)
                    get_filename_component(GRAPHVIZ_LIBRARY_NAME "${GRAPHVIZ_LIBRARY_CANDIDATE}" NAME)
                    if (GRAPHVIZ_LIBRARY_NAME MATCHES "^${BASENAME}-([0-9]+)\\.lib$"
                        AND CMAKE_MATCH_1 GREATER GRAPHVIZ_VERSIONED_LIBRARY_VERSION)
                        set(GRAPHVIZ_VERSIONED_LIBRARY "${GRAPHVIZ_LIBRARY_CANDIDATE}")
                        set(GRAPHVIZ_VERSIONED_LIBRARY_VERSION "${CMAKE_MATCH_1}")
                    endif()
                endforeach()
            endforeach()
        endforeach()

        if (GRAPHVIZ_VERSIONED_LIBRARY)
            set(${OUTVAR} "${GRAPHVIZ_VERSIONED_LIBRARY}" PARENT_SCOPE)
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
