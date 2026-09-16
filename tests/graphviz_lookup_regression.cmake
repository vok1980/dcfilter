include(${CMAKE_CURRENT_LIST_DIR}/../src/graphviz_windows.cmake)

file(REMOVE_RECURSE "${CMAKE_CURRENT_BINARY_DIR}/graphviz_lookup_regression")
file(MAKE_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/graphviz_lookup_regression/lib")
set(GRAPHVIZ_SEARCH_ROOTS "${CMAKE_CURRENT_BINARY_DIR}/graphviz_lookup_regression")

file(WRITE "${GRAPHVIZ_SEARCH_ROOTS}/lib/cgraph.lib" "placeholder")
file(WRITE "${GRAPHVIZ_SEARCH_ROOTS}/lib/cgraph-12.lib" "placeholder")
dcfilter_find_graphviz_library(GRAPHVIZ_CGRAPH_LIBRARY cgraph)
if(NOT GRAPHVIZ_CGRAPH_LIBRARY MATCHES "cgraph\\.lib$")
    message(FATAL_ERROR "Expected exact Graphviz library basename to win")
endif()

file(REMOVE "${GRAPHVIZ_SEARCH_ROOTS}/lib/cgraph.lib")
file(WRITE "${GRAPHVIZ_SEARCH_ROOTS}/lib/cgraph-6.lib" "placeholder")
file(WRITE "${GRAPHVIZ_SEARCH_ROOTS}/lib/gvc-6.lib" "placeholder")
file(WRITE "${GRAPHVIZ_SEARCH_ROOTS}/lib/gvc-12.lib" "placeholder")
unset(GRAPHVIZ_CGRAPH_LIBRARY CACHE)
unset(GRAPHVIZ_CGRAPH_LIBRARY)
unset(GRAPHVIZ_GVC_LIBRARY CACHE)
unset(GRAPHVIZ_GVC_LIBRARY)
dcfilter_find_graphviz_library(GRAPHVIZ_CGRAPH_LIBRARY cgraph)
dcfilter_find_graphviz_library(GRAPHVIZ_GVC_LIBRARY gvc)
if(NOT GRAPHVIZ_CGRAPH_LIBRARY MATCHES "cgraph-12\\.lib$"
    OR NOT GRAPHVIZ_GVC_LIBRARY MATCHES "gvc-12\\.lib$")
    message(FATAL_ERROR "Expected highest versioned Graphviz libraries to win")
endif()

file(REMOVE_RECURSE "${GRAPHVIZ_SEARCH_ROOTS}")
