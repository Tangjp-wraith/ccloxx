#pragma once

#include <any>
#include <cstddef>
#include <string>
#include <vector>
#include "../error/error.h"
#include "../types/token.h"

class Scanner {
 public:
  Scanner(std::string source) : source_(source) {}

  std::vector<Token> scanTokens();

 private:
  void scanToken();
  char advance();
  bool isAtEnd();
  void identifier();
  void number();
  void string();
  void addToken(TokenType type);
  void addToken(TokenType type, std::any literal);
  bool match(char expected);
  char peek();
  char peekNext();
  bool isAlpha(char c);
  bool isAlphaNumeric(char c);
  bool isDigit(char c);

  std::string source_;
  std::vector<Token> tokens_;
  size_t line_{1};
  size_t start_{0};
  size_t current_{0};
};