
execute_process(
    COMMAND ${TARGET} ${OPTIONS}
    INPUT_FILE ${DATA}
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/result.out
    RESULT_VARIABLE CMD_RESULT
)
if(CMD_RESULT)
    message(FATAL_ERROR "Error running ${TARGET} with ${DATA}")
endif()


execute_process(
    COMMAND ${CMAKE_COMMAND} -E compare_files ${CMAKE_CURRENT_BINARY_DIR}/result.out ${REF}
    RESULT_VARIABLE CMD_RESULT
)
if(CMD_RESULT)
    message(FATAL_ERROR "Failed to match files ${CMAKE_CURRENT_BINARY_DIR}/result.out & ${REF}")
endif()
