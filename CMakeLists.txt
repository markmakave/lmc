cmake_minimum_required(VERSION 3.16.0)

project(lmc)

add_executable(lmc src/main.cpp)
target_include_directories(lmc PUBLIC include)
