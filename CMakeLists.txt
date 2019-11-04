project(team16)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "../${CMAKE_DIR}")

set(1_DIR /usr/local/Cellar/sdl2/2.0.10/include)
set(2_DIR /usr/local/Cellar/sdl2_ttf/2.0.15/include)
set(3_DIR /usr/local/Cellar/sdl2_image/2.0.5/include)

include_directories(${1_DIR})
include_directories(${1_DIR}/SDL2)
include_directories(${2_DIR})
include_directories(${3_DIR})

link_directories("/usr/local/Cellar/sdl2/2.0.10/include")
link_directories("/usr/local/Cellar/sdl2_ttf/2.0.15/include")
link_directories("/usr/local/Cellar/sdl2_image/2.0.5/include")

find_library(1_LIB SDL2)
find_library(2_LIB SDL2_image)
find_library(3_LIB SDL2_ttf)

add_executable(team16  text.cpp game.cpp player.cpp playerTexture.cpp bullet.cpp health.cpp healthTexture.cpp bomb.cpp bomb.h)

target_link_libraries(team16 "${1_LIB}")
target_link_libraries(team16 "${2_LIB}")
target_link_libraries(team16 "${3_LIB}")
