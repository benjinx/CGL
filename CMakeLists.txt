cmake_minimum_required(VERSION 3.26)

project(CGL)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_ARCHIVE_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})
set(CMAKE_LIBRARY_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE})

include(FetchContent)

FetchContent_Declare(
    SDL3
    URL "https://github.com/libsdl-org/SDL/archive/refs/heads/main.zip"
    FIND_PACKAGE_ARGS 3
)

FetchContent_Declare(
    glmm
    URL "https://github.com/WhoBrokeTheBuild/glmm/archive/refs/heads/master.zip"
)

FetchContent_MakeAvailable(SDL3 glmm)

find_package(OpenGL REQUIRED)

add_custom_command(
    OUTPUT passthru.vert.spv
    COMMAND glslc -c --target-env=opengl ${CMAKE_SOURCE_DIR}/src/passthru.vert
    DEPENDS ${CMAKE_SOURCE_DIR}/src/passthru.vert
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}
)

add_custom_command(
    OUTPUT halfLambert.frag.spv
    COMMAND glslc -c --target-env=opengl ${CMAKE_SOURCE_DIR}/src/halfLambert.frag
    DEPENDS ${CMAKE_SOURCE_DIR}/src/halfLambert.frag
    WORKING_DIRECTORY ${CMAKE_BINARY_DIR}/${CMAKE_BUILD_TYPE}
)

file(GLOB _srclist src/*)

add_executable(CGL ${_srclist} passthru.vert.spv halfLambert.frag.spv)

target_compile_definitions(CGL PUBLIC _CRT_SECURE_NO_WARNINGS)

target_include_directories(CGL PUBLIC ${CMAKE_SOURCE_DIR}/src)

target_link_libraries(
    CGL
    PUBLIC
        SDL3::SDL3
        OpenGL::GL
        glmm
)