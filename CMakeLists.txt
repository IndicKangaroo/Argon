cmake_minimum_required(VERSION 3.15)

project(argon LANGUAGES C)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

add_executable(arg src/main.c
        src/lexer.c
        src/lexer.h
        src/token.c)

