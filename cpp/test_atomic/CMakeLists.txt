set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -pthread -std=c++11")

message(STATUS "cmake cxx flags : ${CMAKE_CXX_FLAGS}")
message(STATUS "cmake c flags : ${CFLAGS}")

add_executable(test_atomic
    main.cpp
    order_relax.cpp
    order_acquire_release.cpp
    rand.cpp
    )
