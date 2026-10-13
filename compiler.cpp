//
// Created by Zile Xiong on 2026/10/13.
//

#include <iostream>
#include "Lexer.h"
#include "AST.h"

int main() {
    zl::Lexer lexer("/Users/bytedance/Workspace/github/toy-mlir/examples/mul.zl");
    auto vs = lexer.lex();
    zl::ASTContext context(vs);
    context.Parse();
    return 0;
}
