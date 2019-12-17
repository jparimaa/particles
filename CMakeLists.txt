cmake_minimum_required(VERSION 3.10)

project(particles VERSION 0.0.1 LANGUAGES C)

add_subdirectory(external/glfw/)
add_subdirectory(external/glad/)

file(GLOB SOURCES src/*.c include/*.h)
add_executable(particles ${SOURCES})
target_link_libraries(particles PRIVATE glfw glad)
set_property(TARGET particles PROPERTY C_STANDARD 99)

