//
// Created by Zile Xiong on 2026/10/15.
//

#include "MlirGen.h"
namespace mlir::zl {
    ModuleOp* MlirGenImpl::mlirGen(ASTContext &AST) {
        return new ModuleOp();
    }
}
