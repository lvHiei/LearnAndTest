set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g")

message(STATUS "cmake flags : ${CMAKE_CXX_FLAGS}")

add_executable(test
    main.cpp
    )
