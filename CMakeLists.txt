cmake_minimum_required(VERSION 3.12)

project(somecheck)

add_compile_options(-O3 -s)
add_executable(somecheck main.c)