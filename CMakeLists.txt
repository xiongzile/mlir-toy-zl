cmake_minimum_required(VERSION 3.31)
project(toy_mlir LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(MLIR REQUIRED CONFIG)

message(STATUS "Using MLIRConfig.cmake in: ${MLIR_DIR}")
message(STATUS "Using LLVMConfig.cmake in: ${LLVM_DIR}")

list(APPEND CMAKE_MODULE_PATH "${MLIR_CMAKE_DIR}")
list(APPEND CMAKE_MODULE_PATH "${LLVM_CMAKE_DIR}")

include(AddLLVM)
include(AddMLIR)
include(HandleLLVMOptions)

add_definitions(${LLVM_DEFINITIONS})

add_executable(toy_mlir
        compiler.cpp
        Lexer.h
        AST.h
        mlir/MlirGen.cpp
        mlir/MlirGen.h)

target_include_directories(toy_mlir SYSTEM PRIVATE
        ${LLVM_INCLUDE_DIRS}
        ${MLIR_INCLUDE_DIRS})
target_link_libraries(toy_mlir PRIVATE MLIRIR)

llvm_update_compile_flags(toy_mlir)
mlir_check_all_link_libraries(toy_mlir)
