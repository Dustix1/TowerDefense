cmake_minimum_required(VERSION 3.12)
project(TowerDefense C)

set(CMAKE_C_STANDARD 11)

add_executable(sdlgame main.c)

find_package(SDL2 REQUIRED)

target_include_directories(sdlgame PRIVATE ${SDL2_INCLUDE_DIRS})
target_link_libraries(sdlgame SDL2 SDL2_image SDL2_ttf)