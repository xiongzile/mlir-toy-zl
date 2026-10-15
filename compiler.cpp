//
// Created by Zile Xiong on 2026/10/13.
//

#include <mlir/IR/MLIRContext.h>

#include "AST.h"
#include "Lexer.h"
#include "mlir/MlirGen.h"
#include "llvm/Support/CommandLine.h"

namespace cl = llvm::cl;

namespace mlir::zl {
enum ACTION {
  DUMPTOKEN,
  DUMPAST,
  DUMPMLIR,
};
}
static cl::opt<mlir::zl::ACTION>
    emitAction("emit", cl::desc("emit ir or ast"),
               cl::values(clEnumValN(mlir::zl::DUMPAST, "ast", "dump ast")),
               cl::values(clEnumValN(mlir::zl::DUMPMLIR, "ir", "dump ir")),
               cl::values(clEnumValN(mlir::zl::DUMPTOKEN, "tokens", "dump tokens")));

int main(int argc, char *argv[]) {
  cl::ParseCommandLineOptions(argc, argv, "demo tool\n");
  mlir::zl::Lexer lexer("/Users/bytedance/Workspace/github/toy-mlir/examples/mul.zl");
  auto vs = lexer.lex();
  mlir::zl::ASTContext ast(vs);
  ast.Parse();
  mlir::MLIRContext Context;
  auto module = mlir::zl::MlirGenImpl(&Context).mlirGen(ast);

  switch (emitAction) {
  case mlir::zl::DUMPMLIR:
    module->dump();
    llvm::outs() << "dump ir\n";
    break;
  case mlir::zl::DUMPAST:
    ast.Dump();
    llvm::outs() << "dump ast\n";
    break;
  case mlir::zl::DUMPTOKEN:
    lexer.dump();
  }

  return 0;
}
