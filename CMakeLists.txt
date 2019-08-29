cmake_minimum_required (VERSION 3.12)
project (TestApp)
enable_language(C CXX)

set(CMAKE_C_COMPILER=gcc)
set(CMAKE_CXX_COMPILER=g++)
set(CMAKE_CXX_STANDARD 17)

find_package (Threads REQUIRED)
find_package (Protobuf REQUIRED)

enable_testing()

set(CMAKE_MODULE_PATH ${CMAKE_MODULE_PATH} ${CMAKE_SOURCE_DIR}/cmake)
include(CodeCoverage)

add_subdirectory(Common)

add_subdirectory(server)
add_subdirectory(client)
add_subdirectory(utest)


if(${CMAKE_COMPILER_ID} MATCHES GNU AND $my_project_WITH_PROF)
    set_target_properties(my_prog
        PROPERTIES COMPILE_FLAGS "${CMAKE_CXX_FLAGS} -gp -fprofile-coverage -fprofile-args" LINK_FLAGS "-fprofile-arcs")
endif()
