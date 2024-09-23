cmake_minimum_required(VERSION 3.29)
project(ZPG)

set(CMAKE_CXX_STANDARD 17)

add_compile_options(-fsanitize=address)
add_link_options(-fsanitize=address)
set(CMAKE_CXX_FLAGS_INIT "-Werror -Wall -Wextra -Wpedantic -g")

add_executable(ZPG main.cpp)

find_package(glm REQUIRED)
find_package(OpenGL REQUIRED)
target_link_libraries(ZPG glm::glm)
target_link_libraries(ZPG glfw)
target_link_libraries(ZPG OpenGL::GL)