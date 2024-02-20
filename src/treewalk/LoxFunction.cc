#include "LoxFunction.h"

#include "LoxReturn.h"
#include "environment.h"
#include "interpreter.h"
#include "stmt.h"

LoxFunction::LoxFunction(std::shared_ptr<Function> declaration,
                         std::shared_ptr<Environment> closure)
    : declaration{std::move(declaration)}, closure{std::move(closure)} {}
std::string LoxFunction::toString() {
  return "<fn " + declaration->name.lexeme_ + ">";
}

int LoxFunction::arity() { return declaration->params.size(); }

std::any LoxFunction::call(Interpreter& interpreter,
                           std::vector<std::any> arguments) {
  auto environment = std::make_shared<Environment>(closure);
  for (int i = 0; i < declaration->params.size(); ++i) {
    environment->define(declaration->params[i].lexeme_, arguments[i]);
  }
  try {
    interpreter.executeBlock(declaration->body, environment);
  } catch (LoxReturn returnValue) {
    return returnValue.value;
  }
  return nullptr;
}
