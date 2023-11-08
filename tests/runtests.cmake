
execute_process(
    COMMAND ${TARGET}
    INPUT_FILE ${DATA}.in
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/result.out
    RESULT_VARIABLE CMD_RESULT
)
if(CMD_RESULT)
    message(FATAL_ERROR "Error running ${TARGET} with ${DATA}.in")
endif()


execute_process(
    COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/result.out ${DATA}.ref
    RESULT_VARIABLE CMD_RESULT
)
if(CMD_RESULT)
    message(FATAL_ERROR "Failed to match files ${CMAKE_CURRENT_BINARY_DIR}/result.out & ${DATA}.ref")
endif()
