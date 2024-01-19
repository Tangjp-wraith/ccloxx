#pragma once

#include <any>
#include <map>
#include <memory>
#include <string>

#include "../token/token.h"

class Environment : public std::enable_shared_from_this<Environment> {
 public:
  Environment() : enclosing(nullptr) {}
  Environment(std::shared_ptr<Environment> enclosing)
      : enclosing(std::move(enclosing)) {}

  void define(const std::string& name, std::any value);
  void assign(const Token& name, std::any value);
  std::any get(const Token& name);

 private:
  std::map<std::string, std::any> values;
  std::shared_ptr<Environment> enclosing;
};