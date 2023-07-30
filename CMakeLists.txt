cmake_minimum_required(VERSION 3.26)
project(chess LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(LIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/vendor)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

file(GLOB_RECURSE SOURCES ${SRC_DIR}/*.cpp)
add_executable(${PROJECT_NAME} ${SOURCES})

add_compile_options(-Wall)

target_link_libraries(${PROJECT_NAME} PRIVATE sfml-graphics sfml-audio)

add_subdirectory(${LIB_DIR}/sfml)

add_custom_target(copy_assets
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_CURRENT_BINARY_DIR}/assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_SOURCE_DIR}/assets ${CMAKE_CURRENT_BINARY_DIR}/assets
)

if(WIN32 AND BUILD_SHARED_LIBS)
	add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:${PROJECT_NAME}> $<TARGET_FILE_DIR:${PROJECT_NAME}> COMMAND_EXPAND_LISTS
	)
endif()

install(TARGETS ${PROJECT_NAME})
add_dependencies(${PROJECT_NAME} copy_assets)
