cmake_minimum_required(VERSION 3.10)

project(particles VERSION 0.0.1 LANGUAGES C)

add_subdirectory(external/glfw/)
add_subdirectory(external/glad/)

file(GLOB SOURCES src/*.c include/*.h)
add_executable(particles ${SOURCES})
target_link_libraries(particles PRIVATE glfw glad)
target_include_directories(particles PRIVATE external/cglm/include/ external/stb/ include/)
set_property(TARGET particles PROPERTY C_STANDARD 99)

add_definitions(-DSHADER_PATH="${CMAKE_CURRENT_SOURCE_DIR}/shaders/" -DASSET_PATH="${CMAKE_CURRENT_SOURCE_DIR}/assets/")

if (MSVC)
    target_compile_options(particles PRIVATE /W3 /WX /MP)
    target_compile_definitions(particles PRIVATE _CRT_SECURE_NO_WARNINGS)
else()
    target_compile_options(particles PRIVATE -Wall -Wextra -pedantic -Werror)
endif()
