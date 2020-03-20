set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11")

message(STATUS "cmake cxx flags : ${CMAKE_CXX_FLAGS}")
message(STATUS "cmake c flags : ${CFLAGS}")

add_executable(test
    main.cpp
    )
