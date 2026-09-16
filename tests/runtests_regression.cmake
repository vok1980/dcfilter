set(RUNTESTS_SCRIPT ${CMAKE_CURRENT_LIST_DIR}/runtests.cmake)
set(RUNTESTS_TMP_DIR ${CMAKE_CURRENT_BINARY_DIR}/runtests_regression)
set(RESULT_FILE ${RUNTESTS_TMP_DIR}/result.out)
set(REF_FILE ${RUNTESTS_TMP_DIR}/ref.out)

file(MAKE_DIRECTORY ${RUNTESTS_TMP_DIR})

file(WRITE ${RESULT_FILE} "line1\r\nline2\r\n")
file(WRITE ${REF_FILE} "line1\nline2\n")
execute_process(
    COMMAND ${CMAKE_COMMAND} -DSKIP_COMMAND=TRUE -DRESULT=${RESULT_FILE} -DREF=${REF_FILE} -P ${RUNTESTS_SCRIPT}
    WORKING_DIRECTORY ${RUNTESTS_TMP_DIR}
    RESULT_VARIABLE CMD_RESULT
)
if(CMD_RESULT)
    message(FATAL_ERROR "Expected CRLF-generated output to match LF reference output")
endif()

file(WRITE ${RESULT_FILE} "line1\nline3\n")
execute_process(
    COMMAND ${CMAKE_COMMAND} -DSKIP_COMMAND=TRUE -DRESULT=${RESULT_FILE} -DREF=${REF_FILE} -P ${RUNTESTS_SCRIPT}
    WORKING_DIRECTORY ${RUNTESTS_TMP_DIR}
    RESULT_VARIABLE CMD_RESULT
)
if(NOT CMD_RESULT)
    message(FATAL_ERROR "Expected non-newline content differences to fail")
endif()

file(REMOVE_RECURSE ${RUNTESTS_TMP_DIR})
