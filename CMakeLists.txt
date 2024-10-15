cmake_minimum_required(VERSION 3.28)
project(tgx C)

set(CMAKE_C_STANDARD 17)

find_package(glfw3 REQUIRED)

add_executable(tgx library.c
        glad.c
        glad/glad.h
        GLFW/glfw3.h
        GLFW/glfw3native.h
        KHR/khrplatform.h
        tgx.c
)

target_link_libraries(tgx glfw m)