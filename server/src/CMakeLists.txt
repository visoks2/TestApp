cmake_minimum_required (VERSION 3.12)

add_library(TCPServerLib 
    TCPServer.cpp
    TCPServer.h
)
target_include_directories(TCPServerLib PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>)
