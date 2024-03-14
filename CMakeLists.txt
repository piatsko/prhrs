cmake_minimum_required(VERSION 3.10)

project(Prhrs)

project(Prhrs VERSION 1.0)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED True)

add_executable (Prhrs main.cpp progress_bar.h)

target_include_directories(Prhrs PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}")