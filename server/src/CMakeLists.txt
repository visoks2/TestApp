cmake_minimum_required (VERSION 3.12)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_SOURCE_DIR}/data.proto)

add_library(TCPServerLib 
    TCPServer.cpp
    TCPServer.h
    MessagesHandler.h
    ${PROTO_SRCS} ${PROTO_HDRS}
)
target_link_libraries(TCPServerLib 
    pthread 
    ${Protobuf_LIBRARIES}
    CommonLib
)
target_include_directories(TCPServerLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> 
    ${CMAKE_CURRENT_BINARY_DIR}
)
