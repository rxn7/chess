cmake_minimum_required(VERSION 3.26)

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # .exe and .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # .exe and .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # .so and .dylib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") # .lib and .a
set(SFML_BUILD_NETWORK FALSE)

file(GLOB_RECURSE SOURCES *.cpp)
add_executable(chess ${SOURCES})

target_link_libraries(chess PRIVATE chess-lib)

add_subdirectory(${LIB_DIR}/sfml sfml)
target_link_libraries(chess PRIVATE sfml-graphics sfml-audio)

add_custom_target(copy_assets
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}/assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}/assets
)

if(WIN32 AND BUILD_SHARED_LIBS)
	add_custom_command(TARGET chess POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy $<TARGET_RUNTIME_DLLS:chess> $<TARGET_FILE_DIR:chess> COMMAND_EXPAND_LISTS
	)
endif()

install(TARGETS chess)
add_dependencies(chess copy_assets)
