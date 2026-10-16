//
// Created by Zile Xiong on 2026/10/16.
//
#include "ZLDialect.h"

#include "ZLDialect.cpp.inc"

#define GET_OP_CLASSES
#include "ZLOps.cpp.inc"

void zl::ZlDialect::initialize() {
    addOperations<
  #define GET_OP_LIST
  #include "ZLOps.cpp.inc"
    >();
}

llvm::LogicalResult zl::ConstantOp::verify() {
    return llvm::LogicalResult::success();
}
