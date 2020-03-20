set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -pthread -std=c++11 -fpermissive")

message(STATUS "cmake cxx flags : ${CMAKE_CXX_FLAGS}")
message(STATUS "cmake c flags : ${CMAKE_C_FLAGS}")

include_directories(/usr/local/include)

add_executable(test
    main.cpp
    )
