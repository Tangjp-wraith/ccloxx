#pragma once

#include <iostream>
#include <string>

inline bool hadError = false;

static void report(int line, std::string where, std::string msg) {
  std::cerr << "[line " << line << "] error " << where << ": " << msg << "\n";
  hadError = true;
}

inline void Error(int line, std::string msg) { report(line, "", msg); }
