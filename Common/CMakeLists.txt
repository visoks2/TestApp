cmake_minimum_required (VERSION 3.12)

add_library(CommonLib STATIC
    Exceptions.h
    Common.cpp
    Common.h
)

target_include_directories(CommonLib PUBLIC 
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}> 
    ${CMAKE_CURRENT_BINARY_DIR}
)
