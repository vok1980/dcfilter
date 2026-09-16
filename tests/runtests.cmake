
execute_process(
    COMMAND ${TARGET} ${OPTIONS}
    INPUT_FILE ${DATA}
    OUTPUT_FILE ${CMAKE_CURRENT_BINARY_DIR}/result.out
    RESULT_VARIABLE CMD_RESULT
)
if(CMD_RESULT)
    message(FATAL_ERROR "Error running ${TARGET} with ${DATA}")
endif()


file(READ ${CMAKE_CURRENT_BINARY_DIR}/result.out RESULT_CONTENT HEX)
file(READ ${REF} REF_CONTENT HEX)

string(REPLACE "0d0a" "0a" RESULT_CONTENT "${RESULT_CONTENT}")

if(NOT RESULT_CONTENT STREQUAL REF_CONTENT)
    message(FATAL_ERROR "Failed to match files ${CMAKE_CURRENT_BINARY_DIR}/result.out & ${REF}")
endif()
