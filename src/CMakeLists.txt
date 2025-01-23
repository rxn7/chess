cmake_minimum_required(VERSION 3.26)

set(CMAKE_EXECUTABLE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # .exe and .dll
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # .exe and .dll
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin") # .so and .dylib
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/lib") # .lib and .a

set(SFML_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_DOC OFF CACHE BOOL "" FORCE)
set(SFML_BUILD_NETWORK OFF CACHE BOOL "" FORCE)
set(SFML_STATIC_LIBRARIES ON CACHE BOOL "" FORCE)

add_compile_definitions(SFML_STATIC)

file(GLOB_RECURSE SOURCES *.cpp)
add_executable(chess ${SOURCES})

target_link_libraries(chess PRIVATE chess-lib)

set(IMGUI_SFML_FIND_SFML OFF CACHE BOOL "" FORCE)
set(IMGUI_DIR ${LIB_DIR}/imgui)

add_subdirectory(${LIB_DIR}/imgui-sfml imgui-sfml)

add_subdirectory(${LIB_DIR}/sfml sfml)
target_link_libraries(chess PRIVATE sfml-graphics sfml-audio)
target_link_libraries(chess PRIVATE debug ImGui-SFML)

add_custom_target(copy_assets
	COMMAND ${CMAKE_COMMAND} -E remove_directory ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}/assets
	COMMAND ${CMAKE_COMMAND} -E copy_directory ${PROJECT_SOURCE_DIR}/assets ${CMAKE_EXECUTABLE_OUTPUT_DIRECTORY}/assets
)

if(WIN32)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -static -static-libgcc -static-libstdc++")

    add_custom_command(
        TARGET chess
        COMMENT "Copy OpenAL DLL"
        PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy ${SFML_SOURCE_DIR}/extlibs/bin/$<IF:$<EQUAL:${CMAKE_SIZEOF_VOID_P},8>,x64,x86>/openal32.dll $<TARGET_FILE_DIR:chess>
        VERBATIM)

    SET(DLLS
        libvorbis-0.dll
        libvorbisfile-3.dll
        libvorbisenc-2.dll
        libogg.dll
        libFLAC.dll
        libssp-0.dll
        libfreetype-6.dll
        libbrotlicommon.dll
        libbrotlidec.dll
        libbz2-1.dll
        zlib1.dll
    )


    foreach(DLL ${DLLS})
        add_custom_command(
            TARGET chess
            COMMENT "Copy ${DLL}"
            PRE_BUILD COMMAND ${CMAKE_COMMAND} -E copy /usr/x86_64-w64-mingw32/bin/${DLL} $<TARGET_FILE_DIR:chess>
            VERBATIM
        )
    endforeach()
endif()

add_dependencies(chess copy_assets)
