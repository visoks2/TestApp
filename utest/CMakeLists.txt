cmake_minimum_required (VERSION 3.12)
find_package(GTest REQUIRED)
include(GoogleTest)

# add_executable(test_some main.cpp)
add_executable(tests main.cpp)
target_link_libraries(tests ${GTEST_LIBRARIES} pthread)
target_link_libraries(tests 
    TCPServerLib
    TCPClientLib
    CommonLib
)

gtest_discover_tests(tests)
