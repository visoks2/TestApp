cmake_minimum_required (VERSION 3.12)

add_library(TCPClientLib  
    TCPClient.cpp
    TCPClient.h
)
target_include_directories(TCPClientLib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
