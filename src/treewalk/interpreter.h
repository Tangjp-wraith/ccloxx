#pragma once
#include <any>
#include <chrono>
#include <memory>

#include "LoxCallable.h"
#include "environment.h"
#include "expr.h"
#include "stmt.h"

class NativeClock : public LoxCallable {
 public:
  int arity() override { return 0; }
  std::any call(Interpreter& interpreter,
                std::vector<std::any> arguments) override {
    auto ticks = std::chrono::system_clock::now().time_since_epoch();
    return std::chrono::duration<double>{ticks}.count() / 1000.0;
  }
  std::string toString() override { return "<native fn>"; }
};

class Interpreter : public ExprVisitor, StmtVisitor {
 public:
  std::shared_ptr<Environment> globals{new Environment};

 private:
  std::shared_ptr<Environment> environment = globals;

 public:
  Interpreter() { globals->define("clock", std::shared_ptr<NativeClock>{}); };

  void interpret(std::vector<std::shared_ptr<Stmt>>& statements);

  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
  std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
  std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;
  std::any visitVariableExpr(std::shared_ptr<Variable> expr) override;
  std::any visitAssignExpr(std::shared_ptr<Assign> expr) override;
  std::any visitLogicalExpr(std::shared_ptr<Logical> expr) override;
  std::any visitCallExpr(std::shared_ptr<Call> expr) override;

  std::any visitExpressionStmt(std::shared_ptr<Expression> stmt) override;
  std::any visitPrintStmt(std::shared_ptr<Print> stmt) override;
  std::any visitVarStmt(std::shared_ptr<Var> stmt) override;
  std::any visitBlockStmt(std::shared_ptr<Block> stmt) override;
  std::any visitIfStmt(std::shared_ptr<If> stmt) override;
  std::any visitWhileStmt(std::shared_ptr<While> stmt) override;
  std::any visitFunctionStmt(std::shared_ptr<Function> stmt) override;
  std::any visitReturnStmt(std::shared_ptr<Return> stmt) override;

  void executeBlock(const std::vector<std::shared_ptr<Stmt>>& statements,
                    std::shared_ptr<Environment> environment1);

 private:
  std::any evaluate(std::shared_ptr<Expr> expr);
  void execute(std::shared_ptr<Stmt> stmt);
  bool isTruthy(const std::any& object);
  bool isEqual(const std::any& left, const std::any& right);
  void checkNumberOperand(const Token& op, const std::any& operand);
  void checkNumberOperand(const Token& op, const std::any& left,
                          const std::any& right);
  std::string stringify(const std::any& value);
};