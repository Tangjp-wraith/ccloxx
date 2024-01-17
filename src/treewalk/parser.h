#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "expr.h"
#include "../token/token.h"

using ExprPtr = std::shared_ptr<Expr>;

class Parser {
  struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

 public:
  Parser(const std::vector<Token>& tokens) : tokens_{tokens} {}

  ExprPtr parse();

 private:
  ExprPtr expression();
  ExprPtr equality();
  ExprPtr comparison();
  ExprPtr term();
  ExprPtr factor();
  ExprPtr unary();
  ExprPtr primary();

  template <class... T>
  bool match(T... type);
  Token consume(TokenType type, std::string msg);
  Token previous();
  Token advance();
  bool check(TokenType type);
  bool isAtEnd();
  Token peek();
  ParseError error(const Token& token, std::string msg);
  void synchronize();

  const std::vector<Token>& tokens_;
  int current_{0};
};