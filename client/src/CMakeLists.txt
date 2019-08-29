cmake_minimum_required (VERSION 3.12)

protobuf_generate_cpp(PROTO_SRCS PROTO_HDRS ${CMAKE_SOURCE_DIR}/data.proto)

add_library(TCPClientLib  
    TCPClient.cpp
    TCPClient.h
    ${PROTO_SRCS} ${PROTO_HDRS}
)

target_link_libraries(TCPClientLib 
    pthread 
    CommonLib
)

target_include_directories(TCPClientLib PUBLIC  
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> 
    ${CMAKE_CURRENT_BINARY_DIR}
)
