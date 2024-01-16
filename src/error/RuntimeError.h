#pragma once

#include <stdexcept>

#include "../types/token.h"

class RuntimeError : public std::runtime_error {
 public:
  const Token& token_;
  RuntimeError(const Token& token, std::string msg)
      : std::runtime_error{msg.data()}, token_{token} {}
};