#pragma once

#include <iostream>
#include <string>

#include "../types/token.h"
#include "RuntimeError.h"

inline bool hadError = false;
inline bool hadRuntimeError = false;

static void report(int line, std::string where, std::string msg) {
  std::cerr << "[line " << line << "] error " << where << ": " << msg << "\n";
  hadError = true;
}

inline void error(int line, std::string msg) { report(line, "", msg); }

inline void error(const Token& token, std::string msg) {
  if (token.type_ == END_OF_FILE) {
    report(token.line_, "at end", msg);
  } else {
    report(token.line_, " at '" + token.lexeme_ + "'", msg);
  }
}

inline void runtimeError(const RuntimeError& error) {
  std::cerr << error.what() << "\n[line " << error.token_.line_ << "]\n";
  hadRuntimeError = true;
}
