cmake_minimum_required(VERSION 3.30)
project(messenger)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_OSX_ARCHITECTURES "arm64")

if(POLICY CMP0167)
    cmake_policy(SET CMP0167 NEW)
endif()

find_package(Boost REQUIRED COMPONENTS system thread unit_test_framework)

include_directories(${Boost_INCLUDE_DIRS})
link_directories(${Boost_LIBRARY_DIRS})

add_executable(server 
    src/server/chat_room/chat_room.h 
    src/server/chat_room/chat_room.cpp 
    src/server/participant/participant.h 
    src/server/person_in_room/person_in_room.h 
    src/server/person_in_room/person_in_room.cpp 
    src/common.h
    src/server/server.h
    src/server/server.cpp
    src/server/server_main.cpp
)
target_link_libraries(server ${Boost_LIBRARIES})

add_executable(client
    src/client/client.h
    src/client/client.cpp
    src/client/client_main.cpp
    src/common.h
)
target_link_libraries(client ${Boost_LIBRARIES})

enable_testing()

add_executable(server_test 
    src/server/chat_room/chat_room.h 
    src/server/chat_room/chat_room.cpp 
    src/server/participant/participant.h 
    src/server/person_in_room/person_in_room.h 
    src/server/person_in_room/person_in_room.cpp 
    src/common.h
    src/server/server.h
    src/server/server.cpp
    test/server_test.cpp
)
target_link_libraries(server_test ${Boost_LIBRARIES} Boost::unit_test_framework ${Boost_UNIT_TEST_FRAMEWORK_LIBRARY})

add_test(NAME server_tests COMMAND server_test)
