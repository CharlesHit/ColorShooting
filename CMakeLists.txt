project(ColorShooting)

cmake_minimum_required(VERSION 3.15)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY "${PROJECT_SOURCE_DIR}")

include_directories(
        ${PROJECT_SOURCE_DIR}/header
        ${PROJECT_SOURCE_DIR}/source
        ${PROJECT_SOURCE_DIR}/asset
)

set(1_DIR /usr/local/Cellar/sdl2/2.0.10/include)
set(2_DIR /usr/local/Cellar/sdl2_ttf/2.0.15/include)
set(3_DIR /usr/local/Cellar/sdl2_image/2.0.5/include)
set(4_DIR /usr/local/Cellar/sdl2_mixer/2.0.4/include)

include_directories(${1_DIR})
include_directories(${1_DIR}/SDL2)
include_directories(${2_DIR})
include_directories(${3_DIR})
include_directories(${4_DIR})

#link_directories("/usr/local/Cellar/sdl2/2.0.10/include")
#link_directories("/usr/local/Cellar/sdl2_ttf/2.0.15/include")
#link_directories("/usr/local/Cellar/sdl2_image/2.0.5/include")
#link_directories("/usr/local/Cellar/sdl2_mixer/2.0.4/include")

find_library(1_LIB SDL2)
find_library(2_LIB SDL2_image)
find_library(3_LIB SDL2_ttf)
find_library(4_LIB SDL2_mixer)

add_executable(ColorShooting source/text.cpp source/game.cpp source/player.cpp source/playerTexture.cpp source/bullet.cpp source/health.cpp source/HPbox.cpp source/bomb.cpp)

target_link_libraries(ColorShooting "${1_LIB}")
target_link_libraries(ColorShooting "${2_LIB}")
target_link_libraries(ColorShooting "${3_LIB}")
target_link_libraries(ColorShooting "${4_LIB}")
