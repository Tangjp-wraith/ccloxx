#pragma once
#include <any>
#include <memory>

#include "expr.h"

class Interpreter : public ExprVisitor {
 public:
  void interpreter(std::shared_ptr<Expr> expression);

  std::any visitLiteralExpr(std::shared_ptr<Literal> expr) override;
  std::any visitGroupingExpr(std::shared_ptr<Grouping> expr) override;
  std::any visitUnaryExpr(std::shared_ptr<Unary> expr) override;
  std::any visitBinaryExpr(std::shared_ptr<Binary> expr) override;

 private:
  std::any evaluate(std::shared_ptr<Expr> expr);
  bool isTruthy(const std::any& object);
  bool isEqual(const std::any& left, const std::any& right);
  void checkNumberOperand(const Token& op, const std::any& operand);
  void checkNumberOperand(const Token& op, const std::any& left,
                          const std::any& right);
  std::string stringify(const std::any& value);
};