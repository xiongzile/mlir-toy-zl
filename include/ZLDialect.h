//
// Created by Zile Xiong on 2026/10/16.
//

#ifndef TOY_MLIR_ZLDIALECT_H
#define TOY_MLIR_ZLDIALECT_H
#include "mlir/Bytecode/BytecodeOpInterface.h"
#include "mlir/IR/Builders.h"
#include "mlir/IR/BuiltinTypes.h"
#include "mlir/IR/Dialect.h"
#include "mlir/IR/OpDefinition.h"

#include "ZLDialect.h.inc"

#define GET_OP_CLASSES
#include "ZLOps.h.inc"

#endif // TOY_MLIR_ZLDIALECT_H
