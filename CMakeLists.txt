cmake_minimum_required(VERSION 3.20)
project(CPPProject)

set(CMAKE_CXX_STANDARD 20)

add_executable(CPPProject main.cpp Drum.cpp Drum.h Plugboard.cpp Plugboard.h)
