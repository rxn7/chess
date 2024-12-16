cmake_minimum_required(VERSION 3.26)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR ${PROJECT_SOURCE_DIR}/vendor)
set(SRC_DIR ${PROJECT_SOURCE_DIR}/src)

add_compile_options(-Wall)

add_subdirectory(chess_lib)
add_subdirectory(src)
add_subdirectory(test)
