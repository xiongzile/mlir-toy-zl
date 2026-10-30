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
include_directories(${LLVM_INCLUDE_DIRS} ${MLIR_INCLUDE_DIRS})
set(LLVM_TARGET_DEFINITIONS include/ZLDialect.td)
mlir_tablegen(ZLDialect.h.inc      -gen-dialect-decls)
mlir_tablegen(ZLDialect.cpp.inc    -gen-dialect-defs)
mlir_tablegen(ZLOps.h.inc          -gen-op-decls)
mlir_tablegen(ZLOps.cpp.inc        -gen-op-defs)
add_public_tablegen_target(ZLDialectIncGen)

add_executable(toy_mlir
        compiler.cpp
        Lexer.h
        AST.h
        mlir/MlirGen.cpp
        mlir/MlirGen.h
        include/ZLDialect.h
        mlir/ZLDialect.cpp
)

add_dependencies(toy_mlir ZLDialectIncGen)


target_include_directories(toy_mlir SYSTEM PRIVATE
        ${LLVM_INCLUDE_DIRS}
        ${MLIR_INCLUDE_DIRS})
target_include_directories(toy_mlir PRIVATE
        include
        ${CMAKE_CURRENT_BINARY_DIR})
# generated *.inc land here
target_link_libraries(toy_mlir PRIVATE MLIRIR MLIRFuncDialect)

llvm_update_compile_flags(toy_mlir)
mlir_check_all_link_libraries(toy_mlir)
