cmake_minimum_required(VERSION 3.16)
project(dashigrad LANGUAGES C)
include_directories(src)
add_executable(dashigrad main.c src/darray.h src/darray.c src/set.h src/set.c src/engine.h src/engine.c)

