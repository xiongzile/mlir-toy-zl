//
// Created by Zile Xiong on 2026/10/15.
//

#ifndef TOY_MLIR_MLIRGEN_H
#define TOY_MLIR_MLIRGEN_H
#include "../AST.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/BuiltinOps.h"

namespace mlir::zl {
class MlirGenImpl {
  MLIRContext *Context;
  llvm::SmallVector<func::FuncOp, 8> Funcs;

  void codeGen(OpBuilder *Builder, Module module);
  void codeGen(OpBuilder *Builder, FunctionDecl *F);
  void codeGen(OpBuilder *Builder, FunctionBlock *B);
  void codeGen(OpBuilder *Builder, CallExpr *CE);
  void codeGen(OpBuilder *Builder, VarDecl *VD);
  void codeGen(OpBuilder *Builder, Return *Rt);

public:
  MlirGenImpl(MLIRContext *Ctx) : Context(Ctx) {}
  ModuleOp mlirGen(ASTContext &AST);
};
} // namespace mlir::zl
#endif // TOY_MLIR_MLIRGEN_H
