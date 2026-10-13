cmake_minimum_required(VERSION 3.31)
project(toy_mlir)

set(CMAKE_CXX_STANDARD 14)

add_executable(toy_mlir
        compiler.cpp
        ZLLexer.cpp
        ZLLexer.h)
