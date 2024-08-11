cmake_minimum_required(VERSION 3.28)
project(git_switcher)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_EXE_LINKER_FLAGS "-static")

add_executable(git_switcher
        switcher.cpp)
