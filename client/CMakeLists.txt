cmake_minimum_required (VERSION 3.12)

add_executable(Client 
    main_client.cpp 
    TCPClient.cpp
)
target_link_libraries (Client ${CMAKE_THREAD_LIBS_INIT})