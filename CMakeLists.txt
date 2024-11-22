cmake_minimum_required(VERSION 3.12)
project(TowerDefense C)

set(CMAKE_C_STANDARD 11)
add_compile_options(-lm)
add_link_options(-lm)

file(GLOB SOURCES "*.c" "utils/*.c" "interactions/*.c" "scenes/Menu/*.c" "scenes/Game/*.c" "scenes/Game/Friendly/*.c" "scenes/Game/Enemy/*.c" "scenes/Game/Map/*.c")
add_executable(sdlgame ${SOURCES})

find_package(SDL2 REQUIRED)

target_include_directories(sdlgame PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(sdlgame SDL2 SDL2_image SDL2_ttf)