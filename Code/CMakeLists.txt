cmake_minimum_required(VERSION 3.10)

# set the project name
project(Lab2)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

set(CMAKE_CXX_STANDARD_LIBRARIES "-static-libgcc -static-libstdc++ ${CMAKE_CXX_STANDARD_LIBRARIES}")

# add the executable
add_executable(main main.cpp image.cpp)
