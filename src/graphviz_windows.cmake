function(dcfilter_find_graphviz_library OUTVAR BASENAME)
    find_file(${OUTVAR}
        NAMES ${BASENAME}.lib
        HINTS ${GRAPHVIZ_SEARCH_ROOTS}
        PATH_SUFFIXES lib lib/graphviz
    )
    if (DEFINED ${OUTVAR} AND NOT "${${OUTVAR}}" MATCHES "-NOTFOUND$")
        set(${OUTVAR} "${${OUTVAR}}" PARENT_SCOPE)
        return()
    endif()

    unset(${OUTVAR} CACHE)
    unset(${OUTVAR})
    set(GRAPHVIZ_VERSIONED_LIBRARY "")
    set(GRAPHVIZ_VERSIONED_LIBRARY_VERSION -1)
    string(LENGTH "${BASENAME}" GRAPHVIZ_BASENAME_LENGTH)
    foreach(GRAPHVIZ_SEARCH_ROOT IN LISTS GRAPHVIZ_SEARCH_ROOTS)
        foreach(GRAPHVIZ_LIBRARY_SUFFIX IN ITEMS lib lib/graphviz)
            file(GLOB GRAPHVIZ_LIBRARY_CANDIDATES
                LIST_DIRECTORIES FALSE
                "${GRAPHVIZ_SEARCH_ROOT}/${GRAPHVIZ_LIBRARY_SUFFIX}/${BASENAME}-*.lib"
            )
            foreach(GRAPHVIZ_LIBRARY_CANDIDATE IN LISTS GRAPHVIZ_LIBRARY_CANDIDATES)
                get_filename_component(GRAPHVIZ_LIBRARY_NAME "${GRAPHVIZ_LIBRARY_CANDIDATE}" NAME)
                string(FIND "${GRAPHVIZ_LIBRARY_NAME}" "${BASENAME}-" GRAPHVIZ_VERSION_PREFIX)
                if (GRAPHVIZ_VERSION_PREFIX EQUAL 0)
                    string(LENGTH "${GRAPHVIZ_LIBRARY_NAME}" GRAPHVIZ_LIBRARY_NAME_LENGTH)
                    math(EXPR GRAPHVIZ_VERSION_OFFSET "${GRAPHVIZ_BASENAME_LENGTH} + 1")
                    math(EXPR GRAPHVIZ_VERSION_LENGTH
                        "${GRAPHVIZ_LIBRARY_NAME_LENGTH} - ${GRAPHVIZ_BASENAME_LENGTH} - 5")
                    if (GRAPHVIZ_VERSION_LENGTH GREATER 0)
                        string(SUBSTRING "${GRAPHVIZ_LIBRARY_NAME}"
                            ${GRAPHVIZ_VERSION_OFFSET} ${GRAPHVIZ_VERSION_LENGTH}
                            GRAPHVIZ_LIBRARY_VERSION)
                        if (GRAPHVIZ_LIBRARY_VERSION MATCHES "^[0-9]+$"
                            AND GRAPHVIZ_LIBRARY_VERSION GREATER GRAPHVIZ_VERSIONED_LIBRARY_VERSION)
                            set(GRAPHVIZ_VERSIONED_LIBRARY "${GRAPHVIZ_LIBRARY_CANDIDATE}")
                            set(GRAPHVIZ_VERSIONED_LIBRARY_VERSION "${GRAPHVIZ_LIBRARY_VERSION}")
                        endif()
                    endif()
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
