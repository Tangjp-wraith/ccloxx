#pragma once

#include <stdexcept>
#include <iostream>
#include "../token/token.h"

inline bool hadRuntimeError = false;

class RuntimeError : public std::runtime_error {
 public:
  const Token& token_;
  RuntimeError(const Token& token, std::string msg)
      : std::runtime_error{msg.data()}, token_{token} {}
};

inline void runtimeError(const RuntimeError& error) {
  std::cerr << error.what() << "\n[line " << error.token_.line_ << "]\n";
  hadRuntimeError = true;
}
