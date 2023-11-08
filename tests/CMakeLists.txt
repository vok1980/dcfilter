
macro(ADD_TEST_DCFILTER name)
    add_test(
        NAME ${name}
        COMMAND ${CMAKE_COMMAND}
            -DTARGET=$<TARGET_FILE:dcfilter>
            -DDATA=${CMAKE_CURRENT_SOURCE_DIR}/${name}
        -P ${CMAKE_CURRENT_SOURCE_DIR}/runtests.cmake
    )
endmacro()

add_test_dcfilter(simple)
add_test_dcfilter(three)
