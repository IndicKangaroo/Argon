cmake_minimum_required(VERSION 3.15)

project(argon LANGUAGES C ASM_NASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_ASM_NASM_OBJECT_FORMAT elf64)

add_executable(argon
        src/main.c
        src/lexer.c
        src/parser.c
        src/token.c
        src/codegen.c
)

target_include_directories(argon PRIVATE src)
