
if(NOT DEFINED RESULT)
    set(RESULT ${CMAKE_CURRENT_BINARY_DIR}/result.out)
endif()

if(NOT SKIP_COMMAND)
    execute_process(
        COMMAND ${TARGET} ${OPTIONS}
        INPUT_FILE ${DATA}
        OUTPUT_FILE ${RESULT}
        RESULT_VARIABLE CMD_RESULT
    )
    if(CMD_RESULT)
        message(FATAL_ERROR "Error running ${TARGET} with ${DATA}")
    endif()
endif()


file(READ ${RESULT} RESULT_TEXT)
file(READ ${REF} REF_CONTENT HEX)

string(REPLACE "\r\n" "\n" RESULT_TEXT "${RESULT_TEXT}")
string(HEX "${RESULT_TEXT}" RESULT_CONTENT)

if(NOT RESULT_CONTENT STREQUAL REF_CONTENT)
    message(FATAL_ERROR "Failed to match files ${RESULT} & ${REF}")
endif()
