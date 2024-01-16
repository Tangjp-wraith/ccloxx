#include "interpreter.h"

#include "../error/RuntimeError.h"
#include "../error/error.h"

std::any Interpreter::visitLiteralExpr(std::shared_ptr<Literal> expr) {
  return expr->value;
}
std::any Interpreter::visitGroupingExpr(std::shared_ptr<Grouping> expr) {
  return evaluate(expr->expression);
}
std::any Interpreter::visitUnaryExpr(std::shared_ptr<Unary> expr) {
  std::any right = evaluate(expr->right);
  switch (expr->op.type_) {
    case BANG:
      return !isTruthy(right);
    case MINUS:
      checkNumberOperand(expr->op, right);
      return -std::any_cast<double>(right);
    default:
      return std::any{};
  }
}
std::any Interpreter::visitBinaryExpr(std::shared_ptr<Binary> expr) {
  std::any left = evaluate(expr->left);
  std::any right = evaluate(expr->right);
  switch (expr->op.type_) {
    case BANG_EQUAL:
      return !isEqual(left, right);
    case EQUAL_EQUAL:
      return isEqual(left, right);
    case GREATER:
      checkNumberOperand(expr->op, left, right);
      return std::any_cast<double>(left) > std::any_cast<double>(right);
    case GREATER_EQUAL:
      checkNumberOperand(expr->op, left, right);
      return std::any_cast<double>(left) >= std::any_cast<double>(right);
    case LESS:
      checkNumberOperand(expr->op, left, right);
      return std::any_cast<double>(left) < std::any_cast<double>(right);
    case LESS_EQUAL:
      checkNumberOperand(expr->op, left, right);
      return std::any_cast<double>(left) <= std::any_cast<double>(right);
    case MINUS:
      checkNumberOperand(expr->op, left, right);
      return std::any_cast<double>(left) - std::any_cast<double>(right);
    case PLUS:
      if (left.type() == typeid(double) && right.type() == typeid(double)) {
        return std::any_cast<double>(left) + std::any_cast<double>(right);
      }
      if (left.type() == typeid(std::string) &&
          right.type() == typeid(std::string)) {
        return std::any_cast<std::string>(left) +
               std::any_cast<std::string>(right);
      }
      throw RuntimeError{expr->op,
                         "Operand must be two numbers or two strings!"};
    case SLASH:
      checkNumberOperand(expr->op, left, right);
      return std::any_cast<double>(left) / std::any_cast<double>(right);
    case STAR:
      checkNumberOperand(expr->op, left, right);
      return std::any_cast<double>(left) * std::any_cast<double>(right);
    default:
      return std::any{};
  }
}
std::any Interpreter::evaluate(std::shared_ptr<Expr> expr) {
  return expr->accept(*this);
}
bool Interpreter::isTruthy(const std::any& object) {
  if (object.type() == typeid(nullptr)) return false;
  if (object.type() == typeid(bool)) {
    return std::any_cast<bool>(object);
  }
  return true;
}
bool Interpreter::isEqual(const std::any& left, const std::any& right) {
  if (left.type() == typeid(nullptr) && right.type() == typeid(nullptr)) {
    return true;
  }
  if (left.type() == typeid(nullptr)) return false;
  if (left.type() == typeid(std::string) &&
      right.type() == typeid(std::string)) {
    return std::any_cast<std::string>(left) ==
           std::any_cast<std::string>(right);
  }
  if (left.type() == typeid(double) && right.type() == typeid(double)) {
    return std::any_cast<double>(left) == std::any_cast<double>(right);
  }
  if (left.type() == typeid(bool) && right.type() == typeid(bool)) {
    return std::any_cast<bool>(left) == std::any_cast<bool>(right);
  }
  return false;
}
void Interpreter::checkNumberOperand(const Token& op, const std::any& operand) {
  if (operand.type() == typeid(double)) return;
  throw RuntimeError{op, "Operand must be a number"};
}
void Interpreter::checkNumberOperand(const Token& op, const std::any& left,
                                     const std::any& right) {
  if (left.type() == typeid(double) && right.type() == typeid(double)) return;
  throw RuntimeError{op, "Operand must be two numbers!"};
}
void Interpreter::interpreter(std::shared_ptr<Expr> expression) {
  try {
    std::any value = evaluate(expression);
    std::cout << stringify(value) << "\n";
  } catch (RuntimeError error) {
    runtimeError(error);
  }
}
std::string Interpreter::stringify(const std::any& value) {
  if (value.type() == typeid(nullptr)) return "nil";
  if (value.type() == typeid(double)) {
    std::string text = std::to_string(std::any_cast<double>(value));
    int len = text.size();
    if (text[len - 1] == '0' && text[len - 2] == '.') {
      text = text.substr(0, len - 2);
    }
    return text;
  }
  if (value.type() == typeid(std::string)) {
    return std::any_cast<std::string>(value);
  }
  if (value.type() == typeid(bool)) {
    return std::any_cast<bool>(value) ? "true" : "false";
  }
  return "Error in stringify: value type not recognized!";
}