//
// Created by Zile Xiong on 2026/10/13.
//

#include <mlir/IR/MLIRContext.h>

#include "AST.h"
#include "Lexer.h"
#include "mlir/MlirGen.h"
#include "llvm/Support/CommandLine.h"

namespace cl = llvm::cl;

namespace zl {
enum ACTION {
  DUMPTOKEN,
  DUMPAST,
  DUMPMLIR,
};
}
static cl::opt<zl::ACTION>
    emitAction("emit", cl::desc("emit ir or ast"),
               cl::values(clEnumValN(zl::DUMPAST, "ast", "dump ast")),
               cl::values(clEnumValN(zl::DUMPMLIR, "ir", "dump ir")),
               cl::values(clEnumValN(zl::DUMPTOKEN, "tokens", "dump tokens")));

int main(int argc, char *argv[]) {
  cl::ParseCommandLineOptions(argc, argv, "demo tool\n");
  zl::Lexer lexer("/Users/bytedance/Workspace/github/toy-mlir/examples/mul.zl");
  auto vs = lexer.lex();
  zl::ASTContext ast(vs);
  ast.Parse();
  mlir::MLIRContext Context;
  auto module = zl::MlirGenImpl(&Context).mlirGen(ast);

  switch (emitAction) {
  case zl::DUMPMLIR:
    module.dump();
    llvm::outs() << "dump ir\n";
    break;
  case zl::DUMPAST:
    ast.Dump();
    llvm::outs() << "dump ast\n";
    break;
  case zl::DUMPTOKEN:
    lexer.dump();
  }

  return 0;
}
