
macro(ADD_TEST_DCFILTER_D name)
    add_test(
        NAME ${name}_d
        COMMAND ${CMAKE_COMMAND}
            -DTARGET=$<TARGET_FILE:dcfilter>
            -DOPTIONS=-dv
            -DDATA=${CMAKE_CURRENT_SOURCE_DIR}/${name}.in
            -DREF=${CMAKE_CURRENT_SOURCE_DIR}/${name}_d.ref
        -P ${CMAKE_CURRENT_SOURCE_DIR}/runtests.cmake
    )
endmacro()

macro(ADD_TEST_DCFILTER name)
    add_test(
        NAME ${name}
        COMMAND ${CMAKE_COMMAND}
            -DTARGET=$<TARGET_FILE:dcfilter>
            -DOPTIONS=-v
            -DDATA=${CMAKE_CURRENT_SOURCE_DIR}/${name}.in
            -DREF=${CMAKE_CURRENT_SOURCE_DIR}/${name}.ref
        -P ${CMAKE_CURRENT_SOURCE_DIR}/runtests.cmake
    )
endmacro()

add_test_dcfilter_d(simple)
add_test_dcfilter_d(three)
add_test_dcfilter_d(long_cycle)
add_test_dcfilter_d(two_cycles)
add_test_dcfilter_d(three_cycles)
add_test_dcfilter_d(three_cycles_complicated)

add_test_dcfilter(simple)
add_test_dcfilter(three)
add_test_dcfilter(long_cycle)
add_test_dcfilter(two_cycles)
add_test_dcfilter(three_cycles)
add_test_dcfilter(three_cycles_complicated)


find_package(GTest REQUIRED)

add_executable(bit_tests
    bit_test.cpp
)

target_link_libraries(bit_tests
    bit_array
    GTest::Main
)

gtest_discover_tests(bit_tests)
