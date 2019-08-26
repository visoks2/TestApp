cmake_minimum_required (VERSION 3.12)
find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(test main.cpp)
target_link_libraries(test ${GTEST_LIBRARIES} pthread)
target_link_libraries(test TCPServerLib TCPClientLib)
# gtest_add_tests(TARGET      FooTest)

