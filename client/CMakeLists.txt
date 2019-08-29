cmake_minimum_required (VERSION 3.12)

add_subdirectory(src)

add_executable(Client 
    main_client.cpp 
    ${PROTO_SRCS} ${PROTO_HDRS}
)
target_link_libraries (
    Client 
    ${CMAKE_THREAD_LIBS_INIT} 
    ${Protobuf_LIBRARIES}
    TCPClientLib
)
