cmake_minimum_required(VERSION 3.18)
project(Progetto_pel)

set(CMAKE_CXX_STANDARD 14)

add_executable(Progetto_pel player/player.hpp player/player.cpp main.cpp)
