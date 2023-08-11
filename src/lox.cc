#include <cstdlib>
#include <cstring>  // std::strerror
#include <fstream>
#include <iostream>
#include <string>

#include "error/error.h"
#include "scanner/scanner.h"
#include "types/token.h"

std::string readFile(std::string path) {
  std::ifstream file{path.data(),
                     std::ios::in | std::ios::binary | std::ios::ate};
  if (!file) {
    std::cerr << "Failed to open file " << path << ": " << std::strerror(errno)
              << "\n";
    std::exit(74);
  }
  std::string contents;
  contents.resize(file.tellg());
  file.seekg(0, std::ios::beg);
  file.read(contents.data(), contents.size());
  return contents;
}

void run(std::string source) {
  Scanner scanner{source};
  std::vector<Token> tokens = scanner.scanTokens();

  // For now, just print the tokens.
  for (const Token& token : tokens) {
    std::cout << token.toString() << "\n";
  }
}

void runFile(std::string path) {
  std::string contents = readFile(path);
  run(contents);

  // Indicate an error in the exit code.
  if (hadError) {
    std::exit(65);
  }
}

void runPrompt() {
  while (1) {
    std::cout << "> ";
    std::string line;
    if (!std::getline(std::cin, line)) {
      break;
    }
    run(line);
    hadError = false;
  }
}

int main(int argc, char* argv[]) {
  if (argc > 2) {
    std::cout << "Usage ./lox [script] \n";
    std::exit(64);
  } else if (argc == 2) {
    runFile(argv[1]);
  } else {
    runPrompt();
  }
}