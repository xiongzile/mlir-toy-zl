//
// Created by Zile Xiong on 2026/10/15.
//

#include "MlirGen.h"
#include "mlir/Dialect/Func/IR/FuncOps.h"
#include "mlir/IR/Builders.h"

namespace mlir::zl {
ModuleOp MlirGenImpl::mlirGen(ASTContext &AST) {
  OpBuilder builder(Context);

  // module {}
  auto Module = ModuleOp::create(builder.getUnknownLoc(), "top");
  builder.setInsertionPointToEnd(Module.getBody());
  codeGen(&builder, AST.AST);
  return Module;
}

void MlirGenImpl::codeGen(OpBuilder *Builder, Module module) {
}

} // namespace mlir::zl
