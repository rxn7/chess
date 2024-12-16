cmake_minimum_required(VERSION 3.26)

add_subdirectory(${LIB_DIR}/gtest gtest)
enable_testing()
include_directories(${gtest_SOURCE_DIR}/include ${gtest_SOURCE_DIR})

add_executable(chess_test test.cpp)
target_link_libraries(chess_test gtest gtest_main chess-lib)
add_test(chess_test chess_test)
