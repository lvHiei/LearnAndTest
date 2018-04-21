set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread")

message(STATUS "cmake flags : ${CMAKE_CXX_FLAGS}")

add_executable(test
    main.cpp
    Queue.cpp
    TimeUtil.cpp
    )
