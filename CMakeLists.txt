cmake_minimum_required(VERSION 3.21)
project(World_Cup_System)

set(CMAKE_CXX_STANDARD 14)

include_directories(.)

add_executable(World_Cup_System
        AVLTree.h
        Exception.h
        main23a1.cpp
        Player.cpp
        Player.h
        Teams.cpp
        Teams.h
        wet1util.h
        worldcup23a1.cpp
        worldcup23a1.h)
