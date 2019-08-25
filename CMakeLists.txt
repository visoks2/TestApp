cmake_minimum_required (VERSION 3.12)
project (TestApp)
enable_language(C CXX)

set(CMAKE_C_COMPILER=gcc)
set(CMAKE_CXX_COMPILER=g++)
set(CMAKE_CXX_STANDARD 17)

find_package (Threads)

add_subdirectory(server)
add_subdirectory(client)

