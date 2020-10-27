set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11")
project(byte_interview_test)

message(STATUS "cmake cxx flags : ${CMAKE_CXX_FLAGS}")
message(STATUS "cmake c flags : ${CFLAGS}")

set(TARGET_LIST_DIR ".")

AUX_SOURCE_DIRECTORY(${TARGET_LIST_DIR} TARGET_LIST)

foreach(srcfile IN LISTS TARGET_LIST)
    message(STATUS "xxx src ${srcfile}")
    string(REPLACE "./" "" tmpfile ${srcfile})
    string(REPLACE ".cpp" "" tmpfile ${tmpfile})
    string(REPLACE ".cc" "" target ${tmpfile})
    message(STATUS "xxx target ${target}")
    add_executable(${target}
        ${srcfile}
    )
endforeach()
