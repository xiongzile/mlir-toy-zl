//
// Created by Zile Xiong on 2026/10/15.
//

#ifndef TOY_MLIR_MLIRGEN_H
#define TOY_MLIR_MLIRGEN_H
#include "../AST.h"
#include "mlir/IR/BuiltinOps.h"
namespace zl {
class MlirGenImpl {
  mlir::MLIRContext *Context;

public:
  MlirGenImpl(mlir::MLIRContext *Ctx) : Context(Ctx) {}
  mlir::ModuleOp mlirGen(zl::ASTContext &AST);
};
} // namespace zl
#endif // TOY_MLIR_MLIRGEN_H
