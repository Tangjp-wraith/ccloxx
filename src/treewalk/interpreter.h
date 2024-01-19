#pragma once
#include <any>
#include <memory>

#include "environment.h"
#include "expr.h"
#include "stmt.h"

class Interpreter : public ExprVisitor, StmtVisitor {
 private:
  std::shared_ptr<Environment> environment{new Environment};

 public:
  void interpret(std::vector<std::shared_ptr<Stmt>>& statements);

  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
  std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
  std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
  std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
  std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;

  std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
  std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
  std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
  std::any visitBlockStmt(std::shared_ptr<Block> stmt) override;

 private:
  std::any evaluate(std::shared_ptr<Expr> expr);
  void execute(std::shared_ptr<Stmt> stmt);
  void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements,
                    std::shared_ptr<Environment> environment1);
  bool isTruthy(const std::any& object);
  bool isEqual(const std::any& left, const std::any& right);
  void checkNumberOperand(const Token& op, const std::any& operand);
  void checkNumberOperand(const Token& op, const std::any& left,
                          const std::any& right);
  std::string stringify(const std::any& value);
};