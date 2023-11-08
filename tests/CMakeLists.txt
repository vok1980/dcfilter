
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
add_test_dcfilter(long_cycle)
add_test_dcfilter(two_cycles)
add_test_dcfilter(three_cycles)


include(FetchContent)
FetchContent_Declare(
  googletest
  # Specify the commit you depend on and update it regularly.
  URL https://github.com/google/googletest/archive/5376968f6948923e2411081fd9372e71a59d8e77.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


add_executable(bit_tests
    bit_test.cpp
)

include(GoogleTest)

target_link_libraries(bit_tests
    bit_array
    gtest_main
)

gtest_discover_tests(bit_tests)
