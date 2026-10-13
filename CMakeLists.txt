cmake_minimum_required(VERSION 3.31)
project(toy_mlir)

set(CMAKE_CXX_STANDARD 17)

add_executable(toy_mlir
        compiler.cpp
        Lexer.h
        AST.h)
