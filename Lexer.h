//
// Created by Zile Xiong on 2026/10/13.
//

#ifndef TOY_MLIR_ZLLEXER_H
#define TOY_MLIR_ZLLEXER_H
#include <iostream>
#include <string>
#include <string_view>
#include <vector>

namespace zl {

enum TokenKind {
  ID,
  NUMBER,
  SYM,
  IGNORE,
  END,
};

struct Token {
  std::string Id;
  TokenKind Kd;
};
class Lexer {
  std::string_view FileName;
  char *Buf;
  const char *P;
  TokenKind cur;

public:
  Lexer(const char *path) : FileName(path) {
    FILE *Fd = fopen(FileName.data(), "r");
    Buf = static_cast<char *>(malloc(1024 * 1024));
    int n = fread(Buf, sizeof(char), 1024 * 1024, Fd);
    Buf[n] = 0;
    P = Buf;
  }

  Token nextToken() {
    while (true) {
      if (*P == '#') {
        while (*P && *P != '\n') {
          P++;
        }
      } else {
        while (*P && std::isspace(*P)) {
          P++;
        }
        if (*P != '#')
          break;
      }
    }

    if (*P == 0) {
      return Token{"", END};
    }

    std::string res;
    if (std::isalpha(*P)) {
      while (std::isalnum(*P)) {
        res += *P;
        P++;
      }
      return Token{res, ID};
    }

    if (std::isdigit(*P)) {
      while (std::isdigit(*P)) {
        res += *P;
        P++;
      }

      return Token{res, NUMBER};
    }

    res += *P;
    P++;
    return Token{res, SYM};
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

    return result;
  }

  ~Lexer() { free(Buf); }
};
} // namespace zl

#endif // TOY_MLIR_ZLLEXER_H
