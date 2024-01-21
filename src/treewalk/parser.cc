#include "parser.h"

#include <cassert>
#include <memory>

#include "../utils/error.h"

Token Parser::previous() { return tokens_.at(current_ - 1); }

Token Parser::peek() { return tokens_.at(current_); }

bool Parser::isAtEnd() { return peek().type_ == END_OF_FILE; }

bool Parser::check(TokenType type) {
  if (isAtEnd()) {
    return false;
  }
  return peek().type_ == type;
}

Token Parser::advance() {
  if (!isAtEnd()) {
    ++current_;
  }
  return previous();
}

Token Parser::consume(TokenType type, std::string msg) {
  if (check(type)) {
    return advance();
  }
  throw error(peek(), msg);
}

template <class... T>
bool Parser::match(T... type) {
  assert((... && std::is_same_v<T, TokenType>));
  if ((... || check(type))) {
    advance();
    return true;
  }
  return false;
}

ExprPtr Parser::equality() {
  ExprPtr expr = comparison();
  while (match(BANG_EQUAL, EQUAL_EQUAL)) {
    Token op = previous();
    ExprPtr right = comparison();
    expr = std::make_shared<Binary>(expr, std::move(op), right);
  }
  return expr;
}

ExprPtr Parser::expression() { return assignment(); }

ExprPtr Parser::comparison() {
  ExprPtr expr = term();
  while (match(GREATER, GREATER_EQUAL, LESS, LESS_EQUAL)) {
    Token op = previous();
    ExprPtr right = term();
    expr = std::make_shared<Binary>(expr, std::move(op), right);
  }
  return expr;
}

ExprPtr Parser::term() {
  ExprPtr expr = factor();
  while (match(MINUS, PLUS)) {
    Token op = previous();
    ExprPtr right = factor();
    expr = std::make_shared<Binary>(expr, std::move(op), right);
  }
  return expr;
}

ExprPtr Parser::factor() {
  ExprPtr expr = unary();
  while (match(SLASH, STAR)) {
    Token op = previous();
    ExprPtr right = unary();
    expr = std::make_shared<Binary>(expr, std::move(op), right);
  }
  return expr;
}

ExprPtr Parser::unary() {
  if (match(BANG, MINUS)) {
    Token op = previous();
    ExprPtr right = unary();
    return std::make_shared<Unary>(std::move(op), right);
  }
  return primary();
}

ExprPtr Parser::primary() {
  if (match(FALSE)) {
    return std::make_shared<Literal>(false);
  }
  if (match(TRUE)) {
    return std::make_shared<Literal>(true);
  }
  if (match(NIL)) {
    return std::make_shared<Literal>(nullptr);
  }
  if (match(NUMBER, STRING)) {
    return std::make_shared<Literal>(previous().literal_);
  }
  if (match(LEFT_PAREN)) {
    ExprPtr expr = expression();
    consume(RIGHT_PAREN, "Expect ')' after expression.");
    return std::make_shared<Grouping>(expr);
  }
  if (match(IDENTIFIER)) {
    return std::make_shared<Variable>(previous());
  }
  throw error(peek(), "Expect expression.");
}

ExprPtr Parser::assignment() {
  ExprPtr expr = orExpression();
  if (match(EQUAL)) {
    Token equals = previous();
    ExprPtr value = assignment();
    if (Variable* e = dynamic_cast<Variable*>(expr.get())) {
      Token name = e->name;
      return std::make_shared<Assign>(std::move(name), value);
    }
    error(std::move(equals), "Invalid assignment target!");
  }
  return expr;
}
ExprPtr Parser::orExpression() {
  ExprPtr expr = andExpression();
  while (match(OR)) {
    Token op = previous();
    ExprPtr right = andExpression();
    expr = std::make_shared<Logical>(expr, std::move(op), right);
  }
  return expr;
}
ExprPtr Parser::andExpression() {
  ExprPtr expr = equality();
  while (match(AND)) {
    Token op = previous();
    ExprPtr right = equality();
    expr = std::make_shared<Logical>(expr, std::move(op), right);
  }
  return expr;
}
Parser::ParseError Parser::error(const Token& token, std::string msg) {
  ::error(token, msg);
  return ParseError{""};
}

void Parser::synchronize() {
  advance();
  while (!isAtEnd()) {
    if (previous().type_ == SEMICOLON) {
      return;
    }
    switch (peek().type_) {
      case CLASS:
      case FUN:
      case VAR:
      case FOR:
      case IF:
      case WHILE:
      case PRINT:
      case RETURN:
        return;
      default:
        advance();
    }
  }
}
std::vector<StmtPtr> Parser::parse() {
  std::vector<StmtPtr> statements;
  while (!isAtEnd()) {
    statements.push_back(declaration());
  }
  return statements;
}
StmtPtr Parser::statement() {
  if (match(FOR)) return forStatement();
  if (match(IF)) return ifStatement();
  if (match(PRINT)) return printStatement();
  if (match(WHILE)) return whileStatement();
  if (match(LEFT_BRACE)) return std::make_shared<Block>(block());
  return expressionStatement();
}
StmtPtr Parser::printStatement() {
  ExprPtr value = expression();
  consume(SEMICOLON, "Expect ';' after value!");
  return std::make_shared<Print>(value);
}
StmtPtr Parser::expressionStatement() {
  ExprPtr expr = expression();
  consume(SEMICOLON, "Expect ';' after expression!");
  return std::make_shared<Expression>(expr);
}
StmtPtr Parser::declaration() {
  try {
    if (match(VAR)) return varDeclaration();
    return statement();
  } catch (ParseError error) {
    synchronize();
    return nullptr;
  }
}
StmtPtr Parser::varDeclaration() {
  Token name = consume(IDENTIFIER, "Expect variable name!");
  ExprPtr initializer = nullptr;
  if (match(EQUAL)) {
    initializer = expression();
  }
  consume(SEMICOLON, "Expect ';' after expression!");
  return std::make_shared<Var>(std::move(name), initializer);
}
std::vector<StmtPtr> Parser::block() {
  std::vector<StmtPtr> statements;
  while (!check(RIGHT_BRACE) && !isAtEnd()) {
    statements.push_back(declaration());
  }
  consume(RIGHT_BRACE, "Expect '}' after block!");
  return statements;
}
StmtPtr Parser::ifStatement() {
  consume(LEFT_PAREN, "Expect '(' after if!");
  ExprPtr condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after condition!");
  StmtPtr theBranch = statement();
  StmtPtr elseBranch = nullptr;
  if (match(ELSE)) {
    elseBranch = statement();
  }
  return std::make_shared<If>(condition, theBranch, elseBranch);
}
StmtPtr Parser::whileStatement() {
  consume(LEFT_PAREN, "Expect '(' after while!");
  ExprPtr condition = expression();
  consume(RIGHT_PAREN, "Expect ')' after condition!");
  StmtPtr body = statement();
  return std::make_shared<While>(condition, body);
}
StmtPtr Parser::forStatement() {
  consume(LEFT_PAREN, "Expect '(' after for!");
  StmtPtr initializer;
  if (match(SEMICOLON)) {
    initializer = nullptr;
  } else if (match(VAR)) {
    initializer = varDeclaration();
  } else {
    initializer = expressionStatement();
  }
  ExprPtr condition = nullptr;
  if (!check(SEMICOLON)) {
    condition = expression();
  }
  consume(SEMICOLON, "Expect ';' after loop condition!");
  ExprPtr increment = nullptr;
  if (!check(RIGHT_PAREN)) {
    increment = expression();
  }
  consume(RIGHT_PAREN, "Expect ')' after for clauses");
  StmtPtr body = statement();
  if (increment != nullptr) {
    body = std::make_shared<Block>(
        std::vector<StmtPtr>{body, std::make_shared<Expression>(increment)});
  }
  if (condition == nullptr) {
    condition = std::make_shared<Literal>(true);
  }
  body = std::make_shared<While>(condition, body);
  if (initializer != nullptr) {
    body = std::make_shared<Block>(std::vector<StmtPtr>{initializer, body});
  }
  return body;
}
