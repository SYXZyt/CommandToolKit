cmake_minimum_required(VERSION 3.8)
project(CTK_Test)

set(CMAKE_CXX_STANDARD 17)

add_executable(CTK_Test "main.c")
add_executable(CTK_TestCpp "main.cpp")

target_link_libraries(CTK_Test CTK)
target_link_libraries(CTK_TestCpp CTK)