cmake_minimum_required(VERSION 3.26)

file(GLOB_RECURSE LIB_SOURCES *.cpp)
add_library(chess-lib STATIC ${LIB_SOURCES})
target_include_directories(chess-lib PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include)
