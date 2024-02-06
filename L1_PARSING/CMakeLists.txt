cmake_minimum_required(VERSION 3.26)
project(L1_PARSING)

set(CMAKE_CXX_STANDARD 17)

add_executable(match main.cpp
        ops.cpp
        parser.cpp
        Lexer.cpp
        ops.h)
