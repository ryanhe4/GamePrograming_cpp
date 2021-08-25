cmake_minimum_required(VERSION 3.20)
project(GamePrograming_cpp)

set(CMAKE_CXX_STANDARD 20)

find_package(SDL2 REQUIRED)

add_executable(GamePrograming_cpp main.cpp Game.cpp Game.h)
target_link_libraries(GamePrograming_cpp ${SDL2_LIBRARIES})