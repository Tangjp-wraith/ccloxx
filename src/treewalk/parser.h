#pragma once

#include <iostream>
#include <memory>
#include <stdexcept>
#include <vector>

#include "../token/token.h"
#include "expr.h"
#include "stmt.h"

using ExprPtr = std::shared_ptr<Expr>;
using StmtPtr = std::shared_ptr<Stmt>;

class Parser {
  struct ParseError : public std::runtime_error {
    using std::runtime_error::runtime_error;
  };

 public:
  Parser(const std::vector<Token>& tokens) : tokens_{tokens} {}

  std::vector<StmtPtr> parse();

 private:
  ExprPtr expression();
  ExprPtr equality();
  ExprPtr comparison();
  ExprPtr term();
  ExprPtr factor();
  ExprPtr unary();
  ExprPtr primary();
  ExprPtr assignment();

  StmtPtr statement();
  StmtPtr declaration();
  StmtPtr printStatement();
  StmtPtr expressionStatement();
  StmtPtr varDeclaration();

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
  std::vector<StmtPtr> block();

  const std::vector<Token>& tokens_;
  int current_{0};
};