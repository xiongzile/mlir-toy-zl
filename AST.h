//
// Created by Zile Xiong on 2026/10/13.
//

#ifndef TOY_MLIR_AST_H
#define TOY_MLIR_AST_H
#include <utility>

#include "Lexer.h"

namespace zl {

    struct Value {
        bool IsValid;
        std::string Name;
        Value(bool IsValid, std::string str): IsValid(IsValid), Name(std::move(str)) {}

        Value(): IsValid(false), Name("unknown") {}
    };

    struct ParameterDecl : Value {
        ParameterDecl(std::string str): Value(true, std::move(str)) {}
    };

    struct Variable: Value {

    };

    struct Expr : Value {
        virtual ~Expr() = default;
    };

    struct BinaryExpr : Expr {
        std::string ExprType;
        Expr* Left{};
        Expr* Right{};

        BinaryExpr() = default;
        explicit BinaryExpr(std::string eType, Expr* left, Expr* right):
        ExprType(std::move(eType)), Left(left), Right(right) {}
    };

    struct NumExpr : Expr {
        int Val;

        NumExpr(int n): Val(n) {}
    };

    struct VariableExpr : Expr {
        std::string Val;

        VariableExpr(std::string n): Val(n) {}
    };

    struct ListExpr : Expr {
        bool IsList;
        std::vector<ListExpr*> ListItems;
        std::vector<int> IntItems;

        ListExpr(std::vector<ListExpr*> exprs): IsList(true), ListItems(std::move(exprs)) {}
        ListExpr(): IsList(false) {}
    };


    struct Shape: Value {
        std::vector<int> Lane;

        explicit Shape(std::vector<int> lanes) : Lane(std::move(lanes)) {}
        explicit Shape(): Lane({0}) {}
    };

    struct VarDecl : Value {
        Shape VarShape;
        Expr* ValExpr;

        explicit VarDecl() = default;
    };

    struct Return : Value {
        Expr* Val;
        explicit Return(Expr* e): Val(e) {}
        explicit Return(): Val(nullptr) {}
    };

    struct FunctionBlock {
        std::vector<Value*> Stmts;
    };

    struct FunctionDecl {
        std::string FunctionName;
        std::vector<ParameterDecl> Params;
        FunctionBlock Block;
    };

    struct CallExpr : Expr {
        std::string Callee;
        std::vector<Expr*> Arguments;

        explicit CallExpr(std::string callee, std::vector<Expr*> arguments): Callee(std::move(callee)), Arguments(std::move(arguments)) {}
    };


    struct Module : Value {
        std::vector<FunctionDecl> Functions;

        explicit Module(const std::vector<FunctionDecl> & vector);
        explicit Module() {}
    };

    struct ASTContext {
        std::vector<Token> Tokens;
        int Cur;
        Module AST;
        inline static const std::vector<std::string_view> Ops = {
            "+", "-", "*", "/"
        };

        ASTContext(std::vector<Token> tks): Tokens(std::move(tks)), AST(Module{}) {
            Cur = 0;
        }

        void Parse() {
            while (ParseFunction()) {}
        }

        // (a, b)
        void ParseParameters(std::vector<ParameterDecl>& Params) {
            if (!Params.empty()) {
                handleErr("internal error");
            }
            ConsumeToken("(");

            while (true) {
                Token N = ConsumeToken();
                if (N.Id == ")") break;

                Params.emplace_back(N.Id);
                N = ConsumeToken({",", ")"});
                if (N.Id == ")") break;
            }

        }

        Shape ParseValShape() {
            std::vector<int> ret;
            while (true) {
                Token N = ConsumeToken();
                if (N.Id == ">") break;
                ret.emplace_back(stoi(N.Id));
                if (ConsumeToken().Id != ",") break;
            }
            return Shape{ret};
        }


        // [[1, 2, 3], [4, 5, 6]]
        ListExpr* ParseListExpr() {
            ListExpr* Ret = new ListExpr();
            ConsumeToken("[");
            if (Peek().Id == "[") {
                Ret->IsList = true;
                while (true) {
                    if (Peek().Id == "]") {
                        ConsumeToken();
                        break;
                    }
                    Ret->ListItems.emplace_back(ParseListExpr());
                    Token N = ConsumeToken();
                    if (N.Id == "]") break;
                }

                return Ret;
            }

            Ret->IsList = false;
            while (true) {
                if (Peek().Id == "]") {
                    ConsumeToken();
                    break;
                }
                Token N = ConsumeToken();
                int Val = std::stoi(N.Id);
                Ret->IntItems.emplace_back(Val);
                Token IsEnd = ConsumeToken();
                if (IsEnd.Id == "]") break;
            }
            return Ret;

        }

        Expr* ParsePrimary() {
            std::string_view N = Peek().Id;
            if (N == "[") {
                // list expr
                return ParseListExpr();
            }

            if (Peek(1).Id == "(") {
                // call expr
                Token CalleeName = ConsumeToken();
                std::vector<Expr*> Arguments{};
                ConsumeToken("(");
                while (true) {
                    if (Peek().Id == ")") break;
                    Expr* Argument = ParseExpr();
                    Arguments.emplace_back(Argument);
                    if (Peek().Id == ",") {
                        ConsumeToken();
                    }
                }
                ConsumeToken(")");
                return new CallExpr{CalleeName.Id, Arguments};
            }

            // number or variable expr
            Token Var = ConsumeToken();
            if (std::isdigit(Var.Id[0])) {
                return new NumExpr(std::stoi(Var.Id));
            }
            return new VariableExpr(Var.Id);
        }

        Expr* ParseExpr() {
            Expr* Left = ParsePrimary();
            std::string Id = Peek().Id;
            if (std::any_of(Ops.begin(), Ops.end(), [&](std::string_view str) {
                return str == Id;
            })) {
                ConsumeToken({"+", "-", "*", "/"});
                Expr *Right = ParseExpr();
                return new BinaryExpr(Id, Left, Right);
            }
            return Left;
        }

        VarDecl* ParseValDecl() {
            ConsumeToken("var");
            auto *ret = new VarDecl{};
            Token VarName = ConsumeToken();
            ret->Name = VarName.Id;

            Token N = ConsumeToken({"=", "<"});
            if (N.Id == "=") {
                ret->IsValid = true;
                ret->ValExpr = ParseExpr();
            } else {
                ret->IsValid = true;
                ret->VarShape = ParseValShape();
                ConsumeToken("=");
                ret->ValExpr = ParseExpr();
            }
            return ret;
        }

        Return* ParseReturn() {
            ConsumeToken("return");
            auto *ret = new Return{};
            ret->IsValid = true;
            ret->Val = ParseExpr();
            return ret;
        }

        CallExpr* ParseCall() {
            Token CalleeName = ConsumeToken();
            std::vector<Expr*> Arguments{};
            ConsumeToken("(");
            while (true) {
                if (Peek().Id == ")") break;
                Expr* Argument = ParseExpr();
                Arguments.emplace_back(Argument);
                if (Peek().Id == ",") {
                    ConsumeToken();
                }
            }
            ConsumeToken(")");
            auto *Ret = new CallExpr{CalleeName.Id, Arguments};
            Ret->IsValid = true;
            return Ret;
        }

        // {....}
        Value* ParseStmt() {
            Token N = Peek();
            if (N.Id == "}") return new Value{false, "never_used"};

            if (N.Id == "var") {
                // variable declaration
                return ParseValDecl();
            } else if (N.Id == "return") {
                // return statement
                return ParseReturn();
            } else {
                // call statement
                return ParseCall();
            }
        }

        // {....; ....; ....;}
        void ParseFunctionBody(FunctionBlock& Block) {
            ConsumeToken("{");
            std::vector<Value*> Stmts;
            while (true) {
                if (Peek().Id == "}") break;
                Value* Stmt = ParseStmt();
                ConsumeToken(";");
                if (!Stmt->IsValid) break;

                Stmts.emplace_back(Stmt);
                if (Peek().Id == ";") {
                    ConsumeToken();
                }
            }
            Block.Stmts = Stmts;
            ConsumeToken("}");
        }

        void ParseFunctionDecl(FunctionDecl& Func) {
            std::vector<ParameterDecl> Params;
            ParseParameters(Params);
            FunctionBlock Block;
            ParseFunctionBody(Block);
            Func.Params = Params;
            Func.Block = Block;
        }

        bool ParseFunction() {
            Token N = ConsumeToken({"def", "EOF"});
            if (N.Id == "EOF") return false;

            Token FunctionName = ConsumeToken();
            FunctionDecl Func{FunctionName.Id};
            ParseFunctionDecl(Func);
            AST.Functions.emplace_back(Func);
            return true;
        }

        Token Next() {
            return Tokens[Cur++];
        }

        Token Peek(int i) {
            if (Cur + i >= Tokens.size()) {
                return Token{"", END};
            }
            return Tokens[Cur+i];
        }

        Token Peek() {
            return Tokens[Cur];
        }

        [[noreturn]] static void handleErr(std::initializer_list<std::string_view> expects, std::string real) {
            std::cout << "unexpected token: " << real << "\n" << "expected token:\n";
            for (const auto &expect : expects) {
                std::cout << expect << "\n";
            }
            exit(0);
        }

        [[noreturn]] static void handleErr(std::string msg) {
            std::cout << msg << "\n";
            exit(0);
        }

        Token ConsumeToken(std::string_view expect) {
            return ConsumeToken(std::initializer_list{expect});
        }

        Token ConsumeToken(std::initializer_list<std::string_view> expects) {
            if (Cur >= Tokens.size())
                return Token{"EOF", END};
            Token ret = Next();
            if (std::none_of(expects.begin(), expects.end(), [&](std::string_view item) {
                return item == ret.Id;
            })) {
                handleErr(expects, ret.Id);
            }

            return ret;
        }

        Token ConsumeToken() {
            return Next();
        }

        void Musted(std::initializer_list<std::string_view> expects, int i = 0) {
            Token ret = Peek(i);
            if (std::none_of(expects.begin(), expects.end(), [&](std::string_view item) {
                return item == ret.Id;
            })) {
                handleErr(expects, ret.Id);
            }
        }

        void Musted(std::string_view expect, int i = 0) {
            Musted(std::initializer_list{expect}, i);
        }

    };
}
#endif //TOY_MLIR_AST_H
