//
// Created by Zile Xiong on 2026/10/13.
//

#include <iostream>

#include "ZLLexer.h"

int main() {
    zl::Lexer lexer("/Users/bytedance/Workspace/github/toy-mlir/examples/mul.zl");
    auto vs = lexer.lex();
    for (auto v : vs) {
        std::cout << "id: " << v.Id << "\n";
    }
    return 0;
}
