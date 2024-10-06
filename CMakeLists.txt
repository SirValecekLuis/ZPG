cmake_minimum_required(VERSION 3.29)
project(ZPG)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
set(CMAKE_CXX_FLAGS_INIT "-Werror -Wall -Wextra -Wpedantic -g")

add_executable(ZPG
        main.cpp
        shaders.cpp
        shaders.h
        application.h
        application.cpp
        buffers.cpp
        buffers.h
        model.cpp
        model.h)

find_package(glfw3 CONFIG REQUIRED)
target_link_libraries(ZPG PRIVATE glfw)

#find_package(glm CONFIG REQUIRED)
#target_link_libraries(ZPG PRIVATE glm::glm)

find_package(glew CONFIG REQUIRED)
target_link_libraries(ZPG PRIVATE GLEW::GLEW)
