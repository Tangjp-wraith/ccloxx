#pragma once

#include <any>
#include <memory>
#include <string>

#include "LoxCallable.h"

class Environment;
class Function;

class LoxFunction : public LoxCallable {
 public:
  LoxFunction(std::shared_ptr<Function> declaration,
              std::shared_ptr<Environment> closure);
  std::string toString() override;
  int arity() override;
  std::any call(Interpreter& interpreter,
                std::vector<std::any> arguments) override;

 private:
  std::shared_ptr<Function> declaration;
  std::shared_ptr<Environment> closure;
};