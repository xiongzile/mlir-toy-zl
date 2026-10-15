//
// Created by Zile Xiong on 2026/10/15.
//

#ifndef TOY_MLIR_MLIRGEN_H
#define TOY_MLIR_MLIRGEN_H
#include "../AST.h"
#include "mlir/IR/BuiltinOps.h"
namespace mlir::zl {
class MlirGenImpl {
  MLIRContext *Context;

public:
  MlirGenImpl(MLIRContext *Ctx) : Context(Ctx) {}
  ModuleOp *mlirGen(ASTContext &AST);
};
} // namespace mlir::zl
#endif // TOY_MLIR_MLIRGEN_H
