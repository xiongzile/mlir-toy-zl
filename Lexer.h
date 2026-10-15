//
// Created by Zile Xiong on 2026/10/13.
//

#ifndef TOY_MLIR_ZLLEXER_H
#define TOY_MLIR_ZLLEXER_H
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace mlir::zl {

enum TokenKind {
  ID,
  NUMBER,
  SYM,
  IGNORE,
  END,
};

struct SourceLocation {
  int Line;
  int Col;
  const char* FileName;
};

struct Token {
  std::string Id;
  TokenKind Kd;
  SourceLocation Loc;
};
class Lexer {
  std::string_view FileName;
  char *Buf;
  const char *P;
  int Line;
  int Col;
  std::vector<Token> context;

public:
  Lexer(const char *path) : FileName(path) {
    FILE *Fd = fopen(FileName.data(), "r");
    Buf = static_cast<char *>(malloc(1024 * 1024));
    int n = fread(Buf, sizeof(char), 1024 * 1024, Fd);
    Buf[n] = 0;
    P = Buf;
    Line = Col = 1;
  }

  Token nextToken() {
    while (true) {
      while (*P && std::isspace(*P)) {
        if (*P == '\n') {
          Line++;
          Col = 1;
        } else {
          Col++;
        }
        P++;
      }

      if (*P == '\n') {
        Line++;
        Col = 1;
        P++;
      } else if (*P == '#') {
        while (*P && *P != '\n') {
          Col++;
          P++;
        }
        P++;
        Line++;
        Col = 1;
      } else {
        break;
      }
    }

    if (*P == 0) {
      return Token{"", END, SourceLocation{Line, Col, FileName.data()}};
    }

    std::string res;
    if (std::isalpha(*P) || *P == '_') {
      int At = Col;
      while (std::isalnum(*P) || *P == '_') {
        res += *P;
        P++;
        Col++;
      }
      return Token{res, ID, SourceLocation{Line, At, FileName.data()}};
    }

    if (std::isdigit(*P)) {
      int At = Col;
      while (std::isdigit(*P)) {
        res += *P;
        P++;
        Col++;
      }
      return Token{res, NUMBER, SourceLocation{Line, At, FileName.data()}};
    }

    int At = Col;
    res += *P;
    P++;
    Col++;
    return Token{res, SYM, SourceLocation{Line, At, FileName.data()}};
  }

  std::vector<Token> lex() {
    std::vector<Token> result{};
    result.reserve(1000);

    while (true) {
      Token tk = nextToken();
      if (tk.Kd == END)
        break;
      result.emplace_back(tk);
    }
    context = result;
    return result;
  }

  void dump() const {
    for (const auto &token : context) {
      std::cout << token.Id << ":" << token.Loc.Line << ":" << token.Loc.Col
                << "\n";
    }
  }

  ~Lexer() { free(Buf); }
};
} // namespace mlir::zl

#endif // TOY_MLIR_ZLLEXER_H
